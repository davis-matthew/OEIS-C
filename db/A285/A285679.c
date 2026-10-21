/*
 * A285679  Positions of 2 in A285677.
 * https://oeis.org/A285679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor((n-1)*phi) - n + 4
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Helper: compute floor(k*phi) for integer k (k >= 0) using only integer arithmetic */
static inline seq_t floor_k_phi(idx_t k)
{
    seq_t kk  = (seq_t)k;            /* cast to seq_t */
    seq_t sq  = kk * kk;             /* k^2 */
    seq_t t   = sq * 5;              /* 5*k^2 */
    seq_t s   = floor_sqrt(t);       /* floor(sqrt(5*k^2)) = floor(k*sqrt5) */
    seq_t fp  = (kk + s) / 2;        /* floor((k + k*sqrt5)/2) = floor(k*phi) */
    return fp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset starts at 1 */
    idx_t k = n - 1;                         /* k = n-1 */
    seq_t fp   = floor_k_phi(k);              /* floor(k*phi) */
    seq_t result = (seq_t)3 * fp - (seq_t)(k + 1) + (seq_t)4;
    return result;
}
