/*
 * A359651  Numbers with exactly three nonzero decimal digits and not ending
 *          with 0.
 * https://oeis.org/A359651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th smallest positive integer whose decimal representation
 * contains exactly three non-zero decimal digits and whose units digit is non-zero.
 * The numbers are listed in increasing order.
 *
 * The algorithm:
 *   1. Determine the number of decimal digits L for the nth term.
 *      For L ≥ 3 the count of such numbers with exactly L digits is
 *          F(L) = 9 * 9 * 9 * (L-2) = 729 (L-2).
 *      The cumulative count up to L digits is
 *          C(L) = 729 * (L-2)(L-1) / 2.
 *      Using binary search we find the minimal L with C(L) ≥ n.
 *   2. Compute the offset of n within the block of L‑digit numbers.
 *   3. Decompose the offset into:
 *        - leading digit a  (1..9, fixed for the block)
 *        - position of the second non‑zero digit pos_other (1..L-2)
 *        - digit b at pos_other (1..9)
 *        - units digit c (1..9)
 *      The order inside the block is:
 *        a (1..9) → pos_other (1..L-2) → b (1..9) → c (1..9).
 *   4. Assemble the value:
 *        a * 10^(L‑1) + b * 10^(pos_other) + c
 *      Using the provided pow10 utility for arbitrary‑precision computations.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* cumulative count of valid numbers with at most L decimal digits (L ≥ 3) */
static seq_t
cumulative(idx_t L)
{
    if (L < 3) {
        return (seq_t)0;
    }
    /* C(L) = 729 * (L-2)(L-1) / 2 */
    unsigned __int128 tmp = (unsigned __int128)(L - 2) * (L - 1);
    tmp /= (unsigned __int128)2;
    tmp *= (unsigned __int128)729;
    return (seq_t)tmp;
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* undefined, but keep sane */
    }

    /* Find the number of digits L for a(n) */
    idx_t lo = 3, hi = 3;
    while (cumulative(hi) < n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cumulative(mid) >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t L = lo;                       /* L ≥ 3 */

    seq_t cum_before = cumulative(L - 1);
    idx_t offset    = (idx_t)((seq_t)n - cum_before); /* 1‑based within block L */

    /* Decompose offset into components */
    idx_t block_a = 81 * (L - 2);      /* combinations for a fixed leading digit */
    idx_t a_index = (offset - 1) / block_a;
    idx_t rem      = (offset - 1) % block_a;

    idx_t pos_other = (rem / 81) + 1;      /* 1 .. L-2 */
    idx_t rem2     = rem % 81;
    idx_t b = (rem2 / 9) + 1;              /* 1 .. 9 */
    idx_t c = (rem2 % 9) + 1;              /* 1 .. 9 */

    /* Assemble the number */
    seq_t value = pow10(L - 1);
    value *= (seq_t)(a_index + 1);

    seq_t mid_value = pow10(pos_other);
    mid_value *= (seq_t)b;
    value += mid_value;

    value += (seq_t)c;

    return value;
}
