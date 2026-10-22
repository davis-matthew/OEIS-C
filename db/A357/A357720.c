/*
 * A357720  Square array T(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. cos( sqrt(k) * log(1+x) ).
 * https://oeis.org/A357720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) by
 *            antidiagonals. The array is defined by T(0,k)=1, T(1,k)=0 and,
 *            for n>=2, T(n,k) = -(2*n-3)*T(n-1,k) - (n*n-4*n+4 + k)*T(n-2,k).
 *            Let s be the largest integer such that s*(s+1)/2 <= n. Then i =
 *            n - s*(s+1)/2, j = s - i, and a(n) = T(i,j)
 * Range:     i8 n <= 33, i16 n <= 63, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the implementation uses a binary search to find s and
   computes T(i,j) by the above recurrence.  */

#include <stdint.h>

static inline idx_t find_diag(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Increase high until the triangular number exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* Binary search for largest s with s*(s+1)/2 <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal number s. */
    idx_t s = find_diag(n);
    unsigned __int128 first = (unsigned __int128)s * (s + 1) / 2;
    idx_t d = (idx_t)((unsigned __int128)n - first);  /* offset inside diagonal */
    idx_t i = d;
    idx_t j = s - d;   /* column index */

    /* Special cases for the recurrence. */
    if (i == 0) return 1;
    if (i == 1) return 0;

    seq_t prev2 = 1;  /* T(0,j) */
    seq_t prev1 = 0;  /* T(1,j) */

    for (idx_t t = 2; t <= i; ++t) {
        /* Coefficient for T(t-1,j).  2*t-3 fits in idx_t. */
        seq_t coeff1 = (seq_t)((unsigned __int128)2 * t - 3);
        /* Coefficient for T(t-2,j).  t*t may overflow 64‑bit, compute in 128‑bit. */
        seq_t coeff2 = (seq_t)((unsigned __int128)t * t -
                               (unsigned __int128)4 * t + 4 + j);

        /* Compute T(t,j).  All operations are performed in seq_t. */
        seq_t cur = -coeff1 * prev1 - coeff2 * prev2;

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
