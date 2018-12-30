#include "utils.h"


double marsaglia_method(double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}


double dot_product(long double* a, long double *b, size_t n)
{
        double sum = 0.0;
        size_t i;
 
        for (i = 0; i < n; i++) {
                sum += a[i] * b[i];
        }
 
        return sum;
}

long long int compute_M()
{
  int base,exponent;
  base = 2;
  exponent = 32;

  long long result = 1;

  while (exponent != 0)
    {
        result *= base;
        --exponent;
    }

    return result - 5;
}


int compute_cube_size(int dimension){

  int base,exponent;
  base = 2;
  exponent = dimension;

  long long result = 1;

  while (exponent != 0)
    {
        result *= base;
        --exponent;
    }

    return result;
}

int* create_random_r(){

  int* r = new int[K];
  for(int i = 0; i<K; i++)
  {
    r[i] = (rand()/(RAND_MAX+1.0))*(MAX_R_RANGE - 1);
  }

  return r;

}

int ckeck_same_g(int* g1,int* g2){

  int flag = 1;
  for(int i=0; i<K; i++)
  {
    if(g1[i]!=g2[i])
    {
      flag = 0;
      break;
    }
  }

  return flag;
}


int convert_bits_to_int(int* g)
{

  int output=0, power=1;
   for (int i=0; i<K; i++)
   {

       output += g[(K-1)-i]*power;
       power *= 2;
   }

   return output;

}

long double cosine_similarity(long double *A, long double *B)
{

    if(same_v(A,B)) return 0.0;
    else
    {
    long double dot = 0.0, denom_a = 0.0, denom_b = 0.0 ;
     for(unsigned int i = 0; i < DATA_VECTOR_SIZE; ++i) {
        dot += A[i] * B[i] ;
        denom_a += A[i] * A[i] ;
        denom_b += B[i] * B[i] ;
    }
    long double sim = (1- (dot / (sqrtl(denom_a) * sqrtl(denom_b)))) ;

    if(isnan(sim))
    {
      cout<<"Error: nan value"<<endl;
      return -1.0;
    }

    return sim;
    }
} 

int hammingDistance(int x, int y) {
        int z  = x ^ y;
        int r = 0;
        for (; z > 0; z >>= 1) {
            r += z & 1;
        }
        return r;
    }

int* next_probe(int& index,int Tablesize,int *dist_array){

  int init_dist = 1;
  int flag = 1;

  while(flag == 1)
  {
  
  for(int i=0; i<Tablesize; i++)
  {
    if(dist_array[i] == init_dist)
    {
      dist_array[i] = -1;
      index = i;
      flag = 0;
      break;
    }
  }

  init_dist++;

  }

  int* updated_array = new int[Tablesize];
  for(int i=0; i<Tablesize; i++) updated_array[i] = dist_array[i];

  delete []dist_array;

  return updated_array;
}


void int_to_bin_digit(unsigned int in, int len_digitis,int* out_digit){

  unsigned int mask = 1U << (len_digitis-1);
  int i;
  for (i = 0; i < len_digitis; i++) {
    out_digit[i] = (in & mask) ? 1 : 0;
    in <<= 1;
  }
}

int same_v(long double* v1, long double* v2)
{
  for(int i=0; i<DATA_VECTOR_SIZE; i++)
  {
    if(v1[i]!=v2[i]) return 0;
  }

  return 1;
}


