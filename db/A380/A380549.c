/*
 * A380549  List of numbers of the form i + 3*j + 4*i*j for i, j >= 1.
 * https://oeis.org/A380549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th integer of the form i + 3*j + 4*i*j with
 *            integers i ≥ 1, j ≥ 1 (OEIS A380549)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 1;

    for (;; ++x) {
        /* the smallest number in the sequence is 8 */
        if (x < 8) continue;

        /* maximum j that can satisfy 3*j < x */
        idx_t max_j = (idx_t)((x - 1) / 3);

        bool representable = false;
        for (idx_t j = 1; j <= max_j; ++j) {
            seq_t num = x - (seq_t)(3 * j);
            seq_t den = 1 + (seq_t)(4 * j);

            if (num <= 0) continue;          /* safety, should not happen */
            if (num % den == 0) {
                seq_t i = num / den;         /* compute i */
                if (i >= 1) {
                    representable = true;
                    break;
                }
            }
        }

        if (representable) {
            ++count;
            if (count == n) {
                return x;
            }
        }
    }
}
