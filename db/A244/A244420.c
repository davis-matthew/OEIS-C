/*
 * A244420  Numerators of coefficient triangle for expansion of x^n in terms
 *          of polynomials Todd(k, x) = T(2*k+1, sqrt(x))/sqrt(x) (A084930),
 *          with the Chebyshev polynomials of the first kind (type T).
 * https://oeis.org/A244420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) is the numerator of binomial(2*r+1, r-offset) / 2^(2*r)
 *            where r = row of the flattened triangle containing n, offset = n
 *            - r(r+1)/2, and col = r-offset, so that the triangle columns are
 *            listed in descending order (m = r, r-1, …, 0)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

/* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2.       */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;                     /* r can never exceed n          */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;                         /* largest mid with mid*(mid+1)/2 <= n */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Locate the row r containing index n.                     */
    idx_t r = find_row(n);

    /* Compute the offset inside that row.                      */
    unsigned __int128 prev128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t   prev    = (idx_t)prev128;
    idx_t   offset  = n - prev;          /* 0 <= offset <= r          */
    idx_t   col     = r - offset;        /* columns are in descending order   */

    /* Binomial coefficient for the (r, col) entry.            */
    seq_t val = binom(2 * r + 1, col);

    /* Remove powers of two up to 2*r (the denominator).       */
    idx_t limit = 2 * r;
    idx_t shift = 0;
    while (((val & 1) == 0) && (shift < limit)) {
        val >>= 1;
        ++shift;
    }
    return val;
}
