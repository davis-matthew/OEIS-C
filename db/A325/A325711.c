/*
 * A325711  Number of separable partitions of n in which the number of
 *          distinct (repeatable) parts <= 2.
 * https://oeis.org/A325711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + #{(x>y>=1, α>=1, β>=1, |α-β|≤1, αx+βy=n)}
 *            where the counts are for partitions with at most two distinct parts
 *            that can be ordered with no identical adjacent parts.
 */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the single-part partition [n] is always separable */
    seq_t total = 1;

    /* loop over all unordered pairs of distinct part sizes x>y */
    for (idx_t y = 1; y < n; ++y) {
        for (idx_t x = y + 1; x < n; ++x) {
            idx_t S = x + y;                /* sum of the two distinct sizes */

            /* α = β case (equal multiplicities) */
            if (n % S == 0) {
                seq_t alpha = (seq_t)n / (seq_t)S;
                if (alpha >= 1)
                    ++total;                   /* counts a partition */
            }

            /* α > β case: β = (n - x) / S */
            if (n > x) {
                idx_t rem = n - x;
                if (rem % S == 0) {
                    seq_t beta = (seq_t)rem / (seq_t)S;
                    if (beta >= 1)
                        ++total;
                }
            }

            /* β > α case: α = (n - y) / S */
            if (n > y) {
                idx_t rem = n - y;
                if (rem % S == 0) {
                    seq_t alpha = (seq_t)rem / (seq_t)S;
                    if (alpha >= 1)
                        ++total;
                }
            }
        }
    }

    return total;
}
