/*
 * A362366  Square array A(n, k), n, k >= 0, read by antidiagonals; A(n, k) is
 *          the least base >= 2 where the sum n + k can be computed without
 *          carry.
 * https://oeis.org/A362366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9958 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least base b ≥ 2 for which addition in base b of the pair (i,j) on
   the antidiagonal of index n has no carry.  a(n) returns that base. */

#include <stdint.h>
#include <stdlib.h>

/* Compute the minimal base for the pair (n0,k0).  s is the diagonal number. */
static seq_t base_for_pair(idx_t n0, idx_t k0, idx_t s)
{
    idx_t limit = (s + 1 > 2) ? (s + 1) : 2;          /* guarantee b ≥ 2   */
    for (idx_t b = 2; b <= limit; ++b) {
        idx_t tn = n0, tk = k0;
        int ok = 1;
        while (tn || tk) {
            if ((tn % b) + (tk % b) >= b) {
                ok = 0;
                break;
            }
            tn /= b;
            tk /= b;
        }
        if (ok)
            return (seq_t)b;
    }
    /* The bound guarantees a solution, but return the upper bound just
       in case. */
    return (seq_t)limit;
}

/* Find the diagonal number s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2.  */
static idx_t diag_sum(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Grow high until the triangular number exceeds n. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search between low (inclusive) and high (exclusive). */
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Locate the coordinates (i,j) corresponding to index n. */
    idx_t s   = diag_sum(n);
    idx_t tri = (idx_t)((unsigned __int128)s * (s + 1) / 2);
    idx_t pos = n - tri;          /* position along the diagonal   */
    idx_t i   = pos;
    idx_t j   = s - pos;

    return base_for_pair(i, j, s);
}
