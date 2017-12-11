/* ------------------------------------------------------ */
/* Serielles Program zum Finden des groessten Elementes   */       
/* in einem Integer-Vektor                                */
/* ------------------------------------------------------ */

#include <stdlib.h>
#include <stdio.h>

#include <omp.h>

#define VEC_DIM 20

int total_checksum=0;

/* fill array with random numbers */
void init_vector(int *vec, int n, char *dat){

  int i;
  FILE *in;

  in = fopen(dat, "r");

  if (in == NULL)
    printf("ERROR: Cannot open file %s!\n",dat);
  else{
    
    for(i=0; i<n; i++)
      fscanf(in,"%d",&vec[i]);
    
    fclose(in);
  }
}

int calc_checksum(int *vec, int n){

  int sum=0;
  int i;

  for(i=0; i<n; i++)
    sum += vec[i];

#pragma omp critical  
  total_checksum += sum;

  return sum;
}

int main (int argc, char **argv){

    int i, j;
    int x=11;
    int tmp;
    int max_val, max_ind;
    int va[VEC_DIM];
    int vb[VEC_DIM];
    int vc[VEC_DIM];
    int vd[VEC_DIM];
    int ve[VEC_DIM];
    int vf[VEC_DIM];
    int check_va, check_vb, check_ve, check_vf;

    /* start vector operations */

#pragma omp parallel sections private(i)
    {    
    /* add constant to va */
#pragma omp section
      {
    init_vector(va, VEC_DIM, "input_va.dat");  
    for(i=0; i<VEC_DIM; ++i)
      va[i] += x;
    int id = omp_get_thread_num();
    printf("I am %d and I add a constant to va.", id);
    check_va = calc_checksum(va, VEC_DIM);
    printf("Checksum of vector a: %d\n",check_va);
      }

#pragma omp section
      {
	init_vector(vb, VEC_DIM, "input_vb.dat");
	/* transpose vb */
	for(i=0; i<VEC_DIM/2; ++i){
	  tmp = vb[i];
	  vb[i] = vb[VEC_DIM-1-i];
	  vb[VEC_DIM-1-i] = tmp;
	}
	check_vb = calc_checksum(vb, VEC_DIM);
	int id = omp_get_thread_num();
	printf("I am %d and I transpose vb.", id);
	printf("Checksum of vector b: %d\n",check_vb);
      }

#pragma omp section
      {
	init_vector(vc, VEC_DIM, "input_vc.dat");
	init_vector(vd, VEC_DIM, "input_vd.dat");
	/* accumulate vc and vd and write to ve */
	for(i=0; i<VEC_DIM; ++i)
	  ve[i] = vc[i] + vd[i];
	int id = omp_get_thread_num();
	printf("I am %d and I accumulate vc and vd and write to ve.", id);
	check_ve = calc_checksum(ve, VEC_DIM);
	printf("Checksum of vector e: %d\n",check_ve);
      }

#pragma omp section
      {
	init_vector(vf, VEC_DIM, "input_vf.dat");
	/* sort vf */
	for(i=0; i<VEC_DIM; ++i){
	  max_val = vf[i];
	  max_ind = i;
	  for(j=i; j<VEC_DIM; ++j){
	    if(vf[j] > max_val){
	      max_val = vf[j];
	      max_ind = i;
	    }
	  }
	  tmp = vf[i];
	  vf[i] = vf[max_ind];
	  vf[max_ind] = tmp;
	}
	int id = omp_get_thread_num();
	printf("I am %d and I sort vf.", id);
	check_vf = calc_checksum(vf, VEC_DIM);
	printf("Checksum of vector f: %d\n",check_vf);
      }
    }

    printf("Total checksum: %d\n",total_checksum);

    return 0;
}
