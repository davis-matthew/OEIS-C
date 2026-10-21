/*
 * A285323  a(n) = A065642(A065642(A019565(n))) / A019565(n).
 * https://oeis.org/A285323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A065642(A065642(A019565(n))) / A019565(n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* count trailing zeros of a 64‑bit integer (n > 0) */
static inline idx_t trailing_zeros(idx_t n) {
    idx_t c = 0;
    while ((n & 1ULL) == 0) {
        ++c;
        n >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* position of the least significant 1‑bit (0‑based) */
    idx_t pos1 = trailing_zeros(n);
    seq_t p1   = get_prime((idx_t)(pos1 + 1));
    seq_t sq   = p1 * p1;

    /* remove the least significant 1‑bit by shifting right */
    idx_t shifted;
    if (pos1 >= 63) {
        /* n is 2^63, shifting by 64 bits is undefined */
        shifted = 0;
    } else {
        shifted = n >> (pos1 + 1);
    }

    /* if there is no second 1‑bit, return the square */
    if (shifted == 0) return sq;

    /* position of the second least significant 1‑bit */
    idx_t tz2   = trailing_zeros(shifted);
    idx_t pos2  = tz2 + pos1 + 1;
    seq_t p2    = get_prime((idx_t)(pos2 + 1));

    return (p2 > sq) ? sq : p2;
}
