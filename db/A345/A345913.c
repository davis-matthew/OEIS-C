/*
 * A345913  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum >= 0.
 * https://oeis.org/A345913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = nth k such that the k-th composition in standard order
 * (row k of A066099) has non‑negative alternating sum.
 * The function enumerates k starting from 0 and selects those whose
 * alternating sum is >= 0; the n‑th such k is returned.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;; ++k) {
        /* Build the differences of positions of 1‑bits in the reversed binary
         * representation of k.  The reversed binary positions are simply
         * the bit indices + 1, counting LSB as position 1. */
        idx_t diff[64];
        int m = 0;
        idx_t prev = 0;

        for (int e = 0; (k >> e) != 0; ++e) {
            if (k & (1ULL << e)) {
                idx_t pos = (idx_t)(e + 1);
                diff[m++] = pos - prev;
                prev = pos;
            }
        }

        /* Compute the alternating sum of the composition parts, which are
         * the differences in reverse order. */
        int64_t sum = 0;   /* fits because each diff <= 64 */
        int sign = 1;
        for (int i = m - 1; i >= 0; --i) {
            sum += sign * (int64_t)diff[i];
            sign = -sign;
        }

        /* If the alternating sum is non‑negative, this k qualifies. */
        if (sum >= 0) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
