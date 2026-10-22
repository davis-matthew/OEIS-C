/*
 * A347688  Let c (resp. C) be the smallest (resp. largest) number that can be
 *          obtained by cyclically permuting the digits of n; a(n) = C - c.
 * https://oeis.org/A347688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = max(cyclic_permutations(n)) - min(cyclic_permutations(n))
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the OEIS sequence A347688 */
seq_t a(idx_t n)
{
    /* Single-digit numbers give 0 */
    if (n < 10)
        return (seq_t)0;

    /* Extract decimal digits into an array (MSD first). */
    uint8_t digits[64];
    uint64_t tmp = (uint64_t)n;
    size_t d = 0;
    while (tmp > 0) {
        digits[d++] = tmp % 10;
        tmp /= 10;
    }
    /* Reverse to get most significant digit first. */
    for (size_t i = 0, j = d - 1; i < j; ++i, --j) {
        uint8_t t = digits[i];
        digits[i] = digits[j];
        digits[j] = t;
    }

    /* Find min and max over all cyclic rotations. */
    seq_t min_val = 0, max_val = 0;
    for (size_t shift = 0; shift < d; ++shift) {
        seq_t val = 0;
        for (size_t k = 0; k < d; ++k)
            val = val * 10 + digits[(shift + k) % d];
        if (shift == 0) {
            min_val = val;
            max_val = val;
        } else {
            if (val < min_val) min_val = val;
            if (val > max_val) max_val = val;
        }
    }

    return max_val - min_val;
}
