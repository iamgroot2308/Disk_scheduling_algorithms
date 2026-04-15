// PREDICTIVE DISK SCHEDULING SIMULATION 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_REQUESTS 200
#define DISK_SIZE 200

typedef struct {
    int sector;
    int served;
} Request;

typedef struct {
    double total_seek;
    double avg_seek;
} Result;

/* ---------------- Utilities ---------------- */
int cmp_asc(const void *a, const void *b){
    return (*(int*)a - *(int*)b);
}

void copy_requests(Request *dst, Request *src, int n){
    for(int i=0; i<n; i++) dst[i] = src[i];
}

/* ---------------- FCFS ---------------- */
Result fcfs(Request *req, int n, int head){
    Result r = {0};
    int cur = head;
    for(int i=0; i<n; i++){
        r.total_seek += abs(req[i].sector - cur);
        cur = req[i].sector;
    }
    r.avg_seek = (n > 0) ? r.total_seek / n : 0;
    return r;
}

/* ---------------- SSTF ---------------- */
Result sstf(Request *req, int n, int head){
    Result r = {0};
    Request local[MAX_REQUESTS];
    copy_requests(local, req, n);

    int cur = head;
    for(int i=0; i<n; i++){
        int best = -1, min = 1e9;
        for(int j=0; j<n; j++){
            if(!local[j].served){
                int d = abs(local[j].sector - cur);
                if(d < min){ min = d; best = j; }
            }
        }
        local[best].served = 1;
        r.total_seek += min;
        cur = local[best].sector;
    }
    r.avg_seek = (n > 0) ? r.total_seek / n : 0;
    return r;
}

/* ---------------- SCAN ---------------- */
// Note: Assumes initial direction is UP (towards higher sectors)
Result scan(Request *req, int n, int head){
    Result r = {0};
    int arr[MAX_REQUESTS];
    for(int i=0; i<n; i++) arr[i] = req[i].sector;
    qsort(arr, n, sizeof(int), cmp_asc);

    int cur = head;
    // Go UP
    for(int i=0; i<n; i++){
        if(arr[i] >= cur){
            r.total_seek += abs(arr[i] - cur);
            cur = arr[i];
        }
    }
    // Hit the end of the disk
    r.total_seek += abs((DISK_SIZE - 1) - cur);
    cur = DISK_SIZE - 1;

    // Go DOWN
    for(int i=n-1; i>=0; i--){
        if(arr[i] < head){
            r.total_seek += abs(arr[i] - cur);
            cur = arr[i];
        }
    }
    r.avg_seek = (n > 0) ? r.total_seek / n : 0;
    return r;
}

/* ---------------- LOOK ---------------- */
// Note: Assumes initial direction is UP
Result look(Request *req, int n, int head){
    Result r = {0};
    int arr[MAX_REQUESTS];
    for(int i=0; i<n; i++) arr[i] = req[i].sector;
    qsort(arr, n, sizeof(int), cmp_asc);

    int cur = head;
    // Go UP
    for(int i=0; i<n; i++){
        if(arr[i] >= cur){
            r.total_seek += abs(arr[i] - cur);
            cur = arr[i];
        }
    }
    // Go DOWN (no hitting the end of the disk)
    for(int i=n-1; i>=0; i--){
        if(arr[i] < head){
            r.total_seek += abs(arr[i] - cur);
            cur = arr[i];
        }
    }
    r.avg_seek = (n > 0) ? r.total_seek / n : 0;
    return r;
}

/* ---------------- PREDICTIVE ---------------- */
Result predictive_arrival(Request *reqs, int n, int head){
    Result r = {0};
    Request local[MAX_REQUESTS];
    copy_requests(local, reqs, n);
    int order[MAX_REQUESTS], len=0;
    int cur = head;
    int served = 0;
    int k = 4; // window size

    while(served < n){
        // Find last k ARRIVED requests
        int count = (served < k) ? served : k;
        int direction = 1;
        double predicted = cur;

        if(count > 0){
            int start = served - count;
            
            // Predict direction based on previous sequence
            if(reqs[served-1].sector > reqs[start].sector)
                direction = 1;
            else
                direction = -1;

            // Predict value (moving average)
            double sum = 0;
            for(int i=start; i<served; i++)
                sum += reqs[i].sector;
            predicted = sum / count;
        }

        // Select the best next request based on score
        int best = -1;
        double best_score = 1e18;

        for(int j=0; j<n; j++){
            if(!local[j].served){
                int sector = local[j].sector;
                int dir = (sector >= cur) ? 1 : -1;
                double dir_penalty = (dir == direction) ? 1.0 : 2.0;
                double val_dist = fabs((double)sector - predicted); 
                double score = dir_penalty * val_dist;
                
                if(score < best_score){
                    best_score = score;
                    best = j;
                }
            }
        }

        if(best == -1) break;

        local[best].served = 1;
        r.total_seek += abs(local[best].sector - cur);
        cur = local[best].sector;
        order[len++]=cur;
        served++;
    }
    r.avg_seek = (n > 0) ? r.total_seek / n : 0;
    
    printf("PREDICTIVE Order: %d ", head);
    for(int j=0; j<len; j++){
        printf("-> %d ", order[j]);
    }
    printf("\n");
    return r;
}

/* ---------------- Generator ---------------- */
void generate(Request *req, int n){
    int i = 0;
    while(i < n){
        int type = rand() % 10;
        // 70% chance to generate a strong sequential pattern
        if(type < 7 && i + 5 < n){ 
            int start = rand() % (DISK_SIZE - 20);
            for(int k=0; k<5; k++){
                req[i].sector = start + 2*k;
                req[i].served = 0;
                i++;
            }
        } else {
            // 30% chance for random sectors
            req[i].sector = rand() % DISK_SIZE;
            req[i].served = 0;
            i++;
        }
    }
}

void input_req(Request *r, int *n, int *head){
    printf("Enter number of requests: ");
    scanf("%d", n);
    printf("Enter Requests separated by space: ");
    for(int i=0; i<*n; i++){
        scanf("%d", &r[i].sector);
        r[i].served = 0;
    }
    printf("Enter initial head position: ");
    scanf("%d", head);
}

/* ---------------- MAIN ---------------- */
int main(){
    srand(time(NULL));
    int n, head;
    Request base[MAX_REQUESTS];

    // Auto-generate mode (Comment these out and uncomment input_req to use manual mode)
    n = 4 + rand() % 12;
    head = rand() % DISK_SIZE;
    generate(base, n);

    // Uncomment the line below for manual input
    // input_req(base, &n, &head);

    printf("\n=== Disk Scheduling Simulation ===\n");
    printf("Total Requests: %d\n", n);
    printf("Initial Head Position: %d\n", head);
    
    printf("\nRequest Queue:\n[ ");
    for(int i=0; i<n; i++){
        printf("%d ", base[i].sector);
    }
    printf("]\n");
    
    // Execute algorithms
    Result r1 = fcfs(base, n, head);
    Result r2 = sstf(base, n, head);
    Result r3 = scan(base, n, head);
    Result r4 = look(base, n, head);
    Result r5 = predictive_arrival(base, n, head);

    // Print Results
    printf("\n--- Performance Results ---\n");
    printf("%-15s %-15s\n", "Algorithm", "Total Seek Operations");
    printf("----------------------------------\n");
    printf("%-15s %.0f\n", "FCFS", r1.total_seek);
    printf("%-15s %.0f\n", "SSTF", r2.total_seek);
    printf("%-15s %.0f\n", "SCAN", r3.total_seek);
    printf("%-15s %.0f\n", "LOOK", r4.total_seek);
    printf("%-15s %.0f\n", "PREDICTIVE", r5.total_seek);
    printf("----------------------------------\n\n");

    return 0;
}
