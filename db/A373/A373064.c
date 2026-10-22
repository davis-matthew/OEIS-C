/*
 * A373064  The mode of the digits of n (using largest mode if multimodal).
 * https://oeis.org/A373064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = mode digit of decimal repr of n (largest digit if tied)
 * Range:     u8 n <= 833, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);

    unsigned char maxf = 0;
    int maxd = 0;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] >= maxf) {          /* >= chooses largest digit on tie */
            maxf = cnt[d];
            maxd = d;
        }
    }
    return (seq_t)maxd;
}
