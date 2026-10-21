/*
 * A253670  Square array read by ascending antidiagonals, T(n, k) =
 *          k!*[x^k](exp(x)*sum(j=0..n, C(2*n,j)*x^j)), n>=0, k>=0.
 * https://oeis.org/A253670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(r,k) of the square array read by ascending
 *            antidiagonals where T(n,k) = Σ_{j=0..min(n,k)} binom(2n,j) ·
 *            (k)_j, with (k)_j = k!/(k-j)! the falling factorial. For a
 *            global index n (starting at 0) we first locate the antidiagonal
 *            s with s(s+1)/2 ≤ n < (s+1)(s+2)/2. Then k = n – s...
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Find the largest s such that s*(s+1)/2 ≤ n. */
static inline idx_t triangular_inverse(idx_t n)
{
    idx_t lo = 0, hi = n;           /* hi is a safe upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute T(n,k) = Σ_{j=0..min(n,k)} binom(2n,j) * (k)_j. */
static inline seq_t T(idx_t n, idx_t k)
{
    if (k == 0)        /* T(n,0) = 1 for any n */
        return (seq_t)1;

    idx_t m = (n < k) ? n : k;          /* number of terms to sum */
    seq_t sum     = 0;
    seq_t falling = 1;                   /* (k)_0 = 1 */

    for (idx_t j = 0; j <= m; ++j) {
        sum += binom(2 * n, j) * falling;
        if (j < m)
            falling *= (seq_t)(k - j);    /* (k)_{j+1} = (k)_j * (k-j) */
    }
    return sum;
}

/* Public entry point required by the framework. */
seq_t a(idx_t n)
{
    /* Locate the antidiagonal containing index n. */
    idx_t s = triangular_inverse(n);
    __int128 start = (__int128)s * (s + 1) / 2;   /* start index of this diago­nal */
    idx_t k = (idx_t)(n - start);                 /* column index (0 ≤ k ≤ s) */
    idx_t r = s - k;                              /* row index  (0 ≤ r ≤ s) */
    return T(r, k);
}
