/*
 * A345018  For each n, append to the sequence n^2 consecutive integers,
 *          starting from n.
 * https://oeis.org/A345018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = r + n - T(r-1) - 1, where r = least integer with
 *            ∑_{i=1}^r i² ≥ n, and T(k) = k(k+1)(2k+1)/6 (the k‑th sum of
 *            squares)
 * Range:     u8 n <= 253, u16 n <= 9992, u32 n <= 9992, u64 n <= 9992,
 *            u128 n <= 9992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of squares up to r: 1² + 2² + … + r².  */
static inline seq_t sum_of_squares(idx_t r)
{
    if (r == 0) return 0;
    seq_t a = r;               /* a = r            */
    seq_t b = r + 1;           /* b = r+1          */
    seq_t c = (seq_t)2 * r + 1;/* c = 2r+1         */

    /* divide by 2 */
    if ((a & 1) == 0)
        a /= 2;
    else if ((b & 1) == 0)
        b /= 2;
    else
        c /= 2;

    /* divide by 3 */
    if (a % 3 == 0)
        a /= 3;
    else if (b % 3 == 0)
        b /= 3;
    else
        c /= 3;

    return a * b * c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* not part of the defined offset, safe guard */

    /* find the row r such that sum_of_squares(r-1) < n ≤ sum_of_squares(r) */
    idx_t low = 1, high = 1;

    /* grow high until we are sure f(high) >= n */
    while (sum_of_squares(high) < n) {
        if (high > ((idx_t)(-1)) >> 1) {
            high = (idx_t)(-1);
            break;
        }
        high <<= 1;
    }

    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (sum_of_squares(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;
    seq_t prev_total = sum_of_squares(r - 1);
    seq_t offset = (seq_t)n - prev_total;   /* 1‑based offset within row r */
    return (seq_t)r + offset - 1;
}
