/*
 * A362777  Triangular array read by rows: T(n,k) = n!*k + 1, n >= 1, 1 <= k
 *          <= n.
 * https://oeis.org/A362777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = r!*k + 1, where r is the unique row with * r*(r-1)/2 < n
 *            <= r*(r+1)/2 and k = n - r*(r-1)/2. * See OEIS A362777
 *            (triangular array T(n,k)=n!*k+1 read by rows)
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                /* for uint64_t */
#include "../utils/factorial.h"     /* seq_t factorial(idx_t n) */

/* Find the row r such that n lies in that row of the triangular array. */
static idx_t triangular_row(idx_t n)
{
    if (n <= 1) return 1;
    idx_t low = 1;
    idx_t high = 2;
    /* grow high until it covers n */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* binary search (low < high) */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based, 0 handled as dummy */

    /* Determine row r and column k for the global index n. */
    idx_t r = triangular_row(n);
    idx_t prev = r * (r - 1) / 2;   /* number of entries before row r */
    idx_t k = n - prev;             /* 1 <= k <= r */

    /* Compute r! (cached via the provided factorial function). */
    seq_t fact = factorial(r);

    /* Return r!*k + 1. */
    return fact * (seq_t)k + 1;
}
