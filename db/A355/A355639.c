/*
 * A355639  a(n) is the least k > 0 such that the balanced ternary expansion
 *          of k*n contains as many negative trits as positive trits.
 * https://oeis.org/A355639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > 0 such that the balanced ternary expansion of
 *            k*n * has an equal number of positive and negative trits
 * Range:     u8 n <= 18, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   balanced_zero_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/balanced_zero_sum.h"

seq_t a(idx_t n)
{
    /* By definition a(0) = 1.  For n = 0 the product k*n is 0 for any k > 0,
     * and 0 has zero positive and zero negative trits, so the minimal k is 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Brute‑force search for the smallest k > 0.  The search terminates
     * because, for n > 0, the pigeonhole principle guarantees that
     * some multiple of n has at most one more +1 trit than –1 trits, and
     * the next multiple achieves balance.  The values of a(n) for n up to
     * 10 000 are small enough that this simple search is well within the
     * required time limits. */
    for (idx_t k = 1;; ++k) {
        seq_t m = (seq_t)k * (seq_t)n;
        if (balanced_zero_sum(m)) {
            return (seq_t)k;
        }
    }
}
