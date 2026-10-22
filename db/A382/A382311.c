/*
 * A382311  Antidiagonal sums of A382310.
 * https://oeis.org/A382311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*(6*n^2 - 30*n - 23)*floor(n-2*sqrt(n+1)) - (12*n -
 *            6)*floor(n - 2*sqrt(n+1))^2 + 4*floor(n - 2*sqrt(n+1))^3 - 84 -
 *            61*n + 45*n^2 - 2*n^3)/6
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square‑root for unsigned 64‑bit values, via binary search */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid) {        /* mid*mid > x */
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* 1.  Compute K = floor(n - 2*sqrt(n+1))  */
    idx_t n1 = n + 1;                     /* n+1 */
    idx_t r  = isqrt_u64(n1);             /* floor(sqrt(n+1)) */

    idx_t ceil2;                          /* ceil(2*sqrt(n+1))  */
    if ((__int128)r * r == (__int128)n1) {
        /* sqrt(n+1) is integer */
        ceil2 = 2 * r;
    } else {
        /* sqrt(n+1) is not integer */
        /* 2*sqrt(n+1) = 2*r + 2*f,   0 < f < 1 */
        /* Determine if 2*f < 1  (i.e. n1 <= r^2 + r)  */
        idx_t t = (idx_t)((__int128)r * r + r);   /* r^2 + r  */
        if (n1 <= t) {
            ceil2 = 2 * r + 1;
        } else {
            ceil2 = 2 * r + 2;
        }
    }

    /* K can be negative; use signed 128‑bit for intermediate calculations */
    __int128 K = (__int128)n - (__int128)ceil2;

    /* 2.  Evaluate the numerator using 128‑bit integers for safety */
    __int128 N = (__int128)n;

    __int128 a_term  = 6*N*N - 30*N - 23;        /* 6*n^2 - 30*n - 23   */
    __int128 term1   = 2 * a_term * K;           /* 2*a_term*K           */

    __int128 b_term  = 12*N - 6;                 /* 12*n - 6             */
    __int128 K2      = K * K;                    /* K^2                  */
    __int128 term2   = b_term * K2;              /* (12*n-6)*K^2         */

    __int128 K3      = K2 * K;                   /* K^3                  */
    __int128 term3   = 4 * K3;                   /* 4*K^3                */

    __int128 term4   = -84 - 61*N + 45*N*N - 
                       2*N*N*N;                 /* remaining polynomial */

    __int128 numer   = term1 - term2 + term3 + term4;

    /* 3.  Divide by 6 and cast back to seq_t */
    seq_t result = (seq_t)(numer / 6);

    return result;
}
