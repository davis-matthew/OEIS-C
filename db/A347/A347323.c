/*
 * A347323  Replace each nonzero digit d of n by (n mod d).
 * https://oeis.org/A347323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = replace each nonzero digit d of n by (n mod d); zero
 *            digits remain unchanged
 * Range:     u8 n <= 375, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t for modulo operations. */
    seq_t nseq = (seq_t)n;

    /* Special case: n == 0 */
    if (n == 0)
        return 0;

    /* Collect decimal digits of n in reverse order (least significant first). */
    uint8_t digits[20];          /* 20 digits are enough for 64‑bit integers. */
    size_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* Build the transformed number from most significant digit to least. */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        uint8_t d = digits[i - 1];
        uint8_t out;
        if (d == 0)            /* zero digits are kept unchanged */
            out = 0;
        else
            out = (uint8_t)(nseq % (seq_t)d);  /* replace by n mod d */
        result = result * 10 + out;
    }

    return result;
}
