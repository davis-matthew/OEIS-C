/*
 * A244812  The hexagonal spiral of Champernowne, read along the 300-degree
 *          ray.
 * https://oeis.org/A244812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit of Champernowne's constant at position (12 n^2 - 23
 *            n + 12)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   pow10_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_idx_t.h"

static seq_t champernowne_digit(idx_t pos) {
    idx_t remaining = pos;
    int d = 1;
    for (;;) {
        idx_t numbers = (d == 1) ? 9 : 9 * pow10_idx_t(d - 1);
        idx_t block_digits = numbers * d;
        if (remaining <= block_digits) break;
        remaining -= block_digits;
        ++d;
    }
    idx_t offset = remaining - 1;            /* zero‑based offset within block */
    idx_t number_index = offset / d;         /* which number of this digit length */
    idx_t digit_idx = offset % d;            /* index of digit within the number (0 = leftmost) */

    idx_t start = (d == 1) ? 1 : pow10_idx_t(d - 1);
    idx_t number = start + number_index;

    idx_t divisor = pow10_idx_t(d - 1 - digit_idx);
    seq_t digit = (seq_t)((number / divisor) % 10);
    return digit;
}

seq_t a(idx_t n) {
    idx_t m = (idx_t)12 * n * n - (idx_t)23 * n + (idx_t)12;
    return champernowne_digit(m);
}
