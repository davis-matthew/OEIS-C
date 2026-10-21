/*
 * A287676  Positions of 1 in A287674; complement of A287675.
 * https://oeis.org/A287676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 2 - 2*floor((n+1)*phi) with phi = (1+sqrt(5))/2
 *            Computation uses only integer arithmetic; no floating point or
 *            math library functions
 * Range:     u8 n <= 6, u16 n <= 2302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Square root on seq_t by binary search.
   Works for any unsigned integer type that supports +, /, <=, >>, <<. */
static inline seq_t isqrt_seq(seq_t x)
{
    if (x < 2) return x;
    seq_t low = 0;
    seq_t high = x;
    while (low + 1 < high) {
        seq_t mid = (low + high) >> 1;  /* mid = floor((low+high)/2) */
        if (mid == 0) break;           /* should not happen except for x=0,1 handled above */
        if (mid <= x / mid) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  n >= 1. */
    if (n == 0) return 0;        /* guard against out‑of‑range calls */

    /* Compute 5*n + 2 in a wide type to avoid overflow for small seq_t. */
    idx_t term1 = 5 * n + 2;

    /* Compute floor((n+1)*phi) = ((n+1) + floor((n+1)*sqrt(5))) / 2 */
    idx_t np1 = n + 1;                     /* n+1 */
    idx_t tidx = 5 * np1 * np1;            /* 5*(n+1)^2  (fits in idx_t for test range) */

    seq_t t = (seq_t)tidx;                 /* cast to seq_t for sqrt function */
    seq_t sqrt_t = isqrt_seq(t);           /* floor((n+1)*sqrt(5)) */

    seq_t np1_seq = (seq_t)np1;            /* cast n+1 to seq_t */
    seq_t floor_phi = (np1_seq + sqrt_t) >> 1; /* integer division by 2 */

    /* Final formula: a(n) = 5*n + 2 - 2*floor_phi */
    seq_t result = (seq_t)term1 - (floor_phi << 1);
    return result;
}
