/*
 * A342133  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of g.f. 1/(1 - 2*k*x + k*x^2).
 * https://oeis.org/A342133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 129, u64 n <= 339,
 *            u128 n <= 848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342133: square array T(n,k) read by antidiagonals.
   Recurrence for the array:
       T(0,k) = 1
       T(1,k) = 2*k
       T(n,k) = k*(2*T(n-1,k) - T(n-2,k))  for n > 1
   The nth term a(n) is T(p,q) where n = d*(d+1)/2 + p,
   with d = p+q the antidiagonal index, p = n - d*(d+1)/2,
   and q = d - p. */

#include <stdint.h>
#include <stddef.h>

/* Find the largest d such that d*(d+1)/2 <= n  (0‑based antidiagonal index). */
static inline idx_t antidiagonal_idx(idx_t n)
{
    idx_t low = 0, high = 1;
    /* grow high until it exceeds the bound */
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* binary search in [low,high) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine antidiagonal and coordinates (p,q). */
    idx_t d = antidiagonal_idx(n);
    __int128 before = (__int128)d * (d + 1) / 2;
    idx_t pos = n - (idx_t)before;    /* position within the diagonal */
    idx_t p = pos;                    /* row index */
    idx_t k = d - pos;                /* column index (k) */

    /* Compute T(p,k) using the recurrence. */
    if (p == 0) return 1;
    if (p == 1) return (seq_t)2 * (seq_t)k;

    seq_t t0 = 1;                      /* T(0,k) */
    seq_t t1 = (seq_t)2 * (seq_t)k;    /* T(1,k) */

    seq_t t2;
    for (idx_t i = 2; i <= p; ++i) {
        t2 = (seq_t)k * ((seq_t)2 * t1 - t0);
        t0 = t1;
        t1 = t2;
    }
    return t1;   /* T(p,k) = a(n) */
}
