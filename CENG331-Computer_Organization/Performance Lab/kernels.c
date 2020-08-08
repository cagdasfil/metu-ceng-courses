/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "",              /* Student ID */

    "",     /* full name */
    "",  /* email address */

    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * CONVOLUTION KERNEL
 ***************/

/******************************************************
 * Your different versions of the convolution functions  go here
 ******************************************************/
/*
 * naive_conv - The naive baseline version of convolution 
 */
char naive_conv_descr[] = "naive_conv: Naive baseline implementation";
void naive_conv(int dim,int *src, int *ker,int *dst) {
    int i,j,k,l;

    for(i = 0; i < dim-8+1; i++)
        for(j = 0; j < dim-8+1; j++) {
            dst[j*dim+i] = 0;
            for(k = 0; k < 8; k++)
                for(l = 0; l < 8; l++) {
                    dst[j*dim+i] = dst[j*dim+i] +src[(j+l)*dim+(i+k)]*ker[l*dim+k];
                }
        }
            
        
}

/* 
 * convolution - Your current working version of convolution
 * IMPORTANT: This is the version you will be graded on
 */
char convolution_descr[] = "Dot product: Current working version";
void convolution(int dim,int *src, int *ker,int *dst) 
{

        naive_conv(dim,src,ker,dst);

}

/*********************************************************************
 * register_conv_functions - Register all of your different versions
 *     of the convolution functions  with the driver by calling the
 *     add_conv_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_conv_functions() {
    add_conv_function(&naive_conv, naive_conv_descr);   
    add_conv_function(&convolution, convolution_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MATRIX MULTIP KERNEL
 ***************/

/******************************************************
 * Your different versions of the matrix multiplications  go here
 ******************************************************/

/* 
 * naive_matrix_multiplication - The naive baseline version of matrix multiplication 
 */
char naive_matrix_multiplication_descr[] = "Naive_matrix_multiplication: Naive baseline implementation";
void naive_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
            dst[j*dim+i]=0;
            for(k = 0; k < dim; k++) 
                dst[j*dim+i] = dst[j*dim+i] + src[j*dim+k]*src2[i+k*dim];
        }    
}


/* 
 * matrix_multiplication - Your current working version of matrix_multiplication
 * IMPORTANT: This is the version you will be graded on
 */
char matrix_multiplication_descr[] = "Matrix multiplications: Current working version";
void matrix_multiplication(int dim,int *src, int *src2,int *dst) 
{

        naive_matrix_multiplication(dim,src,src2,dst);

}

char new_matrix_multiplication_descr[] = "New version";
void new_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k,sum;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++) {
            sum=0;
            for(k = 0; k < dim; k++)
                sum+= src[j*dim+k]*src2[i+k*dim];
            dst[j*dim+i]=sum;
        }
}

char newer_matrix_multiplication_descr[] = "Newer version";
void newer_matrix_multiplication(int dim,int *src, int *src2,int *dst) {

    /*

     int i,j,k,sum,s;

     for(i = 0; i < dim; i++)
         for(j = 0; j < dim; j++) {
             sum=0;
             for(k = 0; k < dim; k++){
                 s = src[j*dim+k]*src2[i+k*dim];
                 sum = sum+s;
             }
             dst[j*dim+i]=sum;
    }
      */


      int i,j,k,sum,kdim;
      int* psrc;

      if(dim%32 == 0){
          for(i = 0; i < dim; i++)
              for(j = 0; j < dim; j++) {
                  sum = 0;
                  for(k = 0; k < dim; k+=32){
                      kdim = i+k*dim;
                      psrc = src + j*dim+k;
                      sum += psrc[0] * src2[kdim];
                      kdim += dim;
                      sum += psrc[1] * src2[kdim];
                      kdim += dim;
                      sum += psrc[2] * src2[kdim];
                      kdim += dim;
                      sum += psrc[3] * src2[kdim];
                      kdim += dim;
                      sum += psrc[4] * src2[kdim];
                      kdim += dim;
                      sum += psrc[5] * src2[kdim];
                      kdim += dim;
                      sum += psrc[6] * src2[kdim];
                      kdim += dim;
                      sum += psrc[7] * src2[kdim];
                      kdim += dim;
                      sum += psrc[8] * src2[kdim];
                      kdim += dim;
                      sum += psrc[9] * src2[kdim];
                      kdim += dim;
                      sum += psrc[10] * src2[kdim];
                      kdim += dim;
                      sum += psrc[11] * src2[kdim];
                      kdim += dim;
                      sum += psrc[12] * src2[kdim];
                      kdim += dim;
                      sum += psrc[13] * src2[kdim];
                      kdim += dim;
                      sum += psrc[14] * src2[kdim];
                      kdim += dim;
                      sum += psrc[15] * src2[kdim];
                      kdim += dim;
                      sum += psrc[16] * src2[kdim];
                      kdim += dim;
                      sum += psrc[17] * src2[kdim];
                      kdim += dim;
                      sum += psrc[18] * src2[kdim];
                      kdim += dim;
                      sum += psrc[19] * src2[kdim];
                      kdim += dim;
                      sum += psrc[20] * src2[kdim];
                      kdim += dim;
                      sum += psrc[21] * src2[kdim];
                      kdim += dim;
                      sum += psrc[22] * src2[kdim];
                      kdim += dim;
                      sum += psrc[23] * src2[kdim];
                      kdim += dim;
                      sum += psrc[24] * src2[kdim];
                      kdim += dim;
                      sum += psrc[25] * src2[kdim];
                      kdim += dim;
                      sum += psrc[26] * src2[kdim];
                      kdim += dim;
                      sum += psrc[27] * src2[kdim];
                      kdim += dim;
                      sum += psrc[28] * src2[kdim];
                      kdim += dim;
                      sum += psrc[29] * src2[kdim];
                      kdim += dim;
                      sum += psrc[30] * src2[kdim];
                      kdim += dim;
                      sum += psrc[31] * src2[kdim];
                      kdim += dim;
                  }
                  dst[j*dim+i]=sum;
              }
      }
      else{
          for(i = 0; i < dim; i++)
              for(j = 0; j < dim; j++) {
                  sum = 0;
                  for(k = 0; k < dim; k+=16){
                      kdim = i+k*dim;
                      psrc = src + j*dim+k;
                      sum += psrc[0] * src2[kdim];
                      kdim += dim;
                      sum += psrc[1] * src2[kdim];
                      kdim += dim;
                      sum += psrc[2] * src2[kdim];
                      kdim += dim;
                      sum += psrc[3] * src2[kdim];
                      kdim += dim;
                      sum += psrc[4] * src2[kdim];
                      kdim += dim;
                      sum += psrc[5] * src2[kdim];
                      kdim += dim;
                      sum += psrc[6] * src2[kdim];
                      kdim += dim;
                      sum += psrc[7] * src2[kdim];
                      kdim += dim;
                      sum += psrc[8] * src2[kdim];
                      kdim += dim;
                      sum += psrc[9] * src2[kdim];
                      kdim += dim;
                      sum += psrc[10] * src2[kdim];
                      kdim += dim;
                      sum += psrc[11] * src2[kdim];
                      kdim += dim;
                      sum += psrc[12] * src2[kdim];
                      kdim += dim;
                      sum += psrc[13] * src2[kdim];
                      kdim += dim;
                      sum += psrc[14] * src2[kdim];
                      kdim += dim;
                      sum += psrc[15] * src2[kdim];
                      kdim += dim;
                  }
                  dst[j*dim+i]=sum;
              }
      }

 }

char newest_matrix_multiplication_descr[] = "Newest version";
void newest_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    for ( int ij = 0 ; ij < dim*dim ; ij++ )
    {
        int i = ij / dim ;
        int j = ij % dim ;

        dst[i*dim+j] = 0 ;
        for ( int k = 0 ; k < dim ; k++ )
            dst[i*dim+j] += src[i*dim+k] * src2[k*dim+j] ;
    }
}

/*********************************************************************
  * register_matrix_multiplication_functions - Register all of your different versions
  *     of the matrix multiplication  with the driver by calling the
  *     add_matrix_multiplication_function() for each test function. When you run the
  *     driver program, it will test and report the performance of each
  *     registered test function.
  *********************************************************************/

void register_matrix_multiplication_functions() {
    add_matrix_multiplication_function(&naive_matrix_multiplication, naive_matrix_multiplication_descr);   
    add_matrix_multiplication_function(&matrix_multiplication, matrix_multiplication_descr);
    add_matrix_multiplication_function(&new_matrix_multiplication, new_matrix_multiplication_descr);
    add_matrix_multiplication_function(&newer_matrix_multiplication, newer_matrix_multiplication_descr);
    add_matrix_multiplication_function(&newest_matrix_multiplication, newest_matrix_multiplication_descr);
    /* ... Register additional test functions here */
}

