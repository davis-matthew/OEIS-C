/*
 * A322790  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{j=0..k} binomial(2*k,2*j)*(n+1)^(k-j)*n^j.
 * https://oeis.org/A322790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*A322699(n)+1; A(n,k) = (4*n+2)*A(n,k-1)-A(n,k-2),
 *            A(n,0)=1, A(n,1)=2*n+1; a(n) is read from the square array
 *            A(n,k) by antidiagonals with the coordinates in each diagonal
 *            ordered with i increasing (k ascending). The function computes
 *            this value for any non‑negative index n
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 79, u64 n <= 213,
 *            u128 n <= 599, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* integer floor square root for 64‑bit values, used by the antidiagonal
   mapping.  It works without floating point and without external
   helpers. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* Return the nth term of OEIS A322790.  The computation is:
   1. Map n to coordinates (i,j) in the square array using antidiagonal
      reading with reversed order inside each diagonal.
   2. Compute A(i,j) with the Chebyshev recurrence. */
seq_t a(idx_t n)
{
    /* 1. Convert the linear index n to the pair (i,j) in the
       antidiagonal reading with i ascending. */
    /* Compute diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2. */
    idx_t m = (idx_t)((__int128)8 * n + 1);          /* 8*n+1, fits in 128-bit */
    idx_t s = isqrt_u64(m);                         /* floor sqrt(8*n+1) */
    idx_t t = (s - 1) >> 1;                         /* floor((s-1)/2) */

    idx_t prev = (idx_t)((__int128)t * (t + 1) / 2); /* count before this diagonal */
    idx_t k = n - prev;                             /* offset within diagonal */
    idx_t i = k;          /* row index */
    idx_t j = t - k;      /* column index */

    /* 2. Compute A(i,j) with Chebyshev recurrence. */
    if (j == 0) return (seq_t)1;                   /* A(i,0)=1 */

    /* Compute factors as seq_t to avoid overflow for large i. */
    seq_t mult_factor = (seq_t)i;
    mult_factor *= (seq_t)4;                       /* 4*i */
    mult_factor += (seq_t)2;                       /* 4*i+2 */

    seq_t t0 = 1;                                  /* T_0 */
    seq_t t1 = (seq_t)i;                           /* 2*i+1 */
    t1 <<= 1;
    t1 += 1;

    if (j == 1) return t1;                         /* A(i,1)=2*i+1 */

    for (idx_t kk = 2; kk <= j; ++kk) {
        seq_t tk = mult_factor * t1 - t0;
        t0 = t1;
        t1 = tk;
    }
    return t1;
}
