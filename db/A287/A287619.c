/*
 * A287619  Number of positive odd solutions to equation x^2 + 39y^2 = 8*(n +
 *          5).
 * https://oeis.org/A287619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of positive odd solutions to x^2 + 39 y^2 = 8 · (n + 5)
 * Range:     u8 n <= 9, u16 n <= 4328, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* base value 8·(n + 5) */
    seq_t rhs_base = 8 * ((seq_t)n + (seq_t)5);

    /* maximal y such that 39 y² ≤ rhs_base  */
    seq_t y_max = isqrt_seq(rhs_base / (seq_t)39);

    seq_t count = 0;

    for (idx_t y = 1; y <= (idx_t)y_max; ++y) {
        seq_t yy = (seq_t)y * (seq_t)y;
        seq_t term = (seq_t)39 * yy;
        if (term > rhs_base) break;            /* further y only increase term */

        seq_t rhs = rhs_base - term;           /* x² candidate */

        seq_t x = isqrt_seq(rhs);
        if (x * x == rhs && (x & (seq_t)1) && x > (seq_t)0) {
            ++count;
        }
    }

    return count;
}
