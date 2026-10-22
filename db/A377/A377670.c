/*
 * A377670  Number of subwords of the form UDD in nondecreasing Dyck paths of
 *          length 2n.
 * https://oeis.org/A377670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*F(2*n-3) + n*L(2*n-3) + 2*L(2*n-2) - 5*2^(n-2))/5
 *            using Fibonacci F(k) and Lucas L(k) numbers. a(0)=0, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* pow2(idx_t) */

/* fast doubling: compute F(n) and F(n+1)
   Parameters:
     n  : index (idx_t)
     fn : pointer to F(n)   (seq_t)
     fn1: pointer to F(n+1) (seq_t)
*/
static inline void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    /* find most significant bit of n */
    idx_t msb = 63 - __builtin_clzll(n);

    seq_t a = 0; /* F(k)   */
    seq_t b = 1; /* F(k+1) */

    for (idx_t i = msb; ; i--) {
        /* doubling formulas:
           F(2k)   = F(k) * [2*F(k+1) – F(k)]
           F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t two_b_minus_a = (b << 1) - a;
        seq_t c = a * two_b_minus_a;         /* F(2k)   */
        seq_t d = a * a + b * b;             /* F(2k+1) */

        if ((n >> i) & 1) {
            a = d;
            b = c + d;
        } else {
            a = c;
            b = d;
        }
        if (i == 0)
            break;
    }
    *fn = a;
    *fn1 = b;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 */
    if (n < 2)
        return 0;

    /* m = 2*n - 3  (>=1 for n>=2) */
    idx_t m = 2 * n - 3;

    seq_t F_m, F_m1;          /* F(m), F(m+1) */
    fib_pair(m, &F_m, &F_m1);

    /* compute F(m+2) = F(m) + F(m+1) */
    seq_t F_m2 = F_m + F_m1;

    /* Lucas numbers:
       L(m)   = 2*F(m+1) - F(m)
       L(m+1) = F(m) + F(m+2)
    */
    seq_t L_m   = (seq_t)2 * F_m1 - F_m;
    seq_t L_m1  = F_m + F_m2;  /* L(m+1) */

    seq_t pow2_term = pow2(n - 2);   /* 2^(n-2) */

    /* numerator = 2*F_m + n*L_m + 2*L_m1 - 5*pow2_term */
    seq_t term1 = (seq_t)2 * F_m;
    seq_t term2 = L_m * (seq_t)n;
    seq_t term3 = (seq_t)2 * L_m1;
    seq_t term4 = (seq_t)5 * pow2_term;

    seq_t sum = term1 + term2 + term3;
    seq_t numer = sum - term4;

    return numer / (seq_t)5;
}
