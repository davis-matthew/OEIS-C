/*
 * A274319  Numbers whose digit sum is divisible by 6.
 * https://oeis.org/A274319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = nth number whose digit sum is divisible by 6; found by
 *            brute‑force scan
 * Range:     u8 n <= 42, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) = 0 */
        return 0;

    idx_t count = 1;   /* 0 already counted as the first term */
    for (seq_t v = 1; ; ++v) {
        if (digit_sum(v) % 6 == 0) {
            ++count;
            if (count == n)
                return v;
        }
    }
}
