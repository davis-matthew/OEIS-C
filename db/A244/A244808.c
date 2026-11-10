/*
 * A244808  The hexagonal spiral of Champernowne, read along the 60-degree
 *          ray.
 * https://oeis.org/A244808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit number 12n^2-27n+16 (1-based) of Champernowne's
 *            constant 0.123456789101112..., i.e. A033307(12n^2-27n+15) with
 *            A033307 0-indexed
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A033307_a(idx_t n) {
    idx_t L = 1;
    idx_t pow10 = 1;

    while (1) {
        idx_t count = 9 * pow10;
        unsigned __int128 seg = (unsigned __int128)count * L;
        if ((unsigned __int128)n < seg)
            break;
        n -= (idx_t)seg;
        ++L;
        pow10 *= 10;
    }

    idx_t offset = n;
    idx_t indexInBlock = offset / L;
    idx_t digitPos = offset % L;

    idx_t number = pow10 + indexInBlock;

    idx_t divisor = pow10;
    for (idx_t i = 0; i < digitPos; ++i)
        divisor /= 10;

    return (seq_t)((number / divisor) % 10);
}

seq_t a(idx_t n) {
    idx_t pos = 12 * n * n - 27 * n + 15; /* 0-based index */
    return A033307_a(pos);
}
