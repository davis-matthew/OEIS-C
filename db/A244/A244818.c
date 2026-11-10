/*
 * A244818  The hexagonal spiral of Champernowne, read along the 120-degree
 *          ray.
 * https://oeis.org/A244818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit at position 12n^2-17n+6 (1-based) of the
 *            Champernowne constant 123456789101112... (A033307)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A033307_digit(idx_t p) {
    /* p is 0-based position in the Champernowne digit string */
    idx_t L = 1;
    idx_t pow10 = 1;
    while (1) {
        unsigned __int128 seg = (unsigned __int128)(9 * pow10) * L;
        if ((unsigned __int128)p < seg) break;
        p -= (idx_t)seg;
        ++L;
        pow10 *= 10;
    }
    idx_t number = pow10 + p / L;
    idx_t digitPos = p % L;
    idx_t divisor = pow10;
    for (idx_t i = 0; i < digitPos; ++i) divisor /= 10;
    return (seq_t)((number / divisor) % 10);
}

seq_t a(idx_t n) {
    idx_t pos = 12 * n * n - 17 * n + 6; /* 1-based */
    return A033307_digit(pos - 1);
}
