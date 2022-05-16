#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

typedef struct thread_data {
   int n;
   int *arr;
   int id;
} thread_data;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

//assumindo que estao ordenados
int *merge(int *l, int n_l, int *r, int n_r){
	int i = 0;
	int j = 0;
	int k = 0;
	int * a = (int*)malloc((n_r+n_l)*sizeof(int));
	while(i < n_l && j < n_r){
		if (l[i] < r[j]) a[k++] = l[i++];
		else a[k++] = r[j++];
	}
	while (i < n_l) a[k++] = l[i++];
	while (j < n_r) a[k++] = r[j++];
	return a;
}

void *sort(void *arg){
	thread_data *tdata = (thread_data *) arg;
	int n = tdata->n;
	int *arr = tdata->arr;
	int id = tdata->id;
	printf("Thread %d - Tamanho do segmento = %d\n", id, n);
	fflush(stdin);
	qsort(arr, n, sizeof(int), cmpfunc);
	pthread_exit(NULL);
}

int *gen(int n, int max){
	srand(time(NULL));
	int *a = (int*)malloc((n)*sizeof(int));
	for (int i = 0; i < n; ++i) a[i] = rand() % max;
	return a;
}

int main(int argc, char **argv){
	
	int n = atoi(argv[1]);
	int *c = gen(n, atoi(argv[3]));	
	int k = atoi(argv[2]);
	int sz = n/k;
	int r = n % k;
	thread_data tdata[k];
    pthread_t threads[k];
    int cur = 0;
	for (int i = 0; i < k; ++i){
		tdata[i].n = sz + (r > 0 ? 1: 0);
		tdata[i].arr = &c[cur];
		cur += tdata[i].n;
		r--;
		tdata[i].id = i;
		pthread_create(&(threads[i]), NULL, sort, &(tdata[i]));
	}
	for (int i = 0; i < k; ++i)
		pthread_join(threads[i], NULL);
	int next = k;
	while (next != 1){
		int x = 0;
		for (int i = 0; i < next - 1; i += 2, x++){
			int l = tdata[i].n;
			int r = tdata[i+1].n;
			tdata[x].n = l + r;
			tdata[x].arr = merge(tdata[i].arr, l, tdata[i+1].arr, r);
		}
		tdata[x] = tdata[next-1];
		if (next & 1) next = next/2 +1;
		else next /= 2;
	}
	return 0;
}
