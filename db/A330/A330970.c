/*
 * A330970  For any number n >= 0 with decimal digits (d_1, d_2, ..., d_k),
 *          the decimal expansion of a(n) is (n mod (1+d_1), n mod (1+d_2),
 *          ..., n mod (1+d_k)).
 * https://oeis.org/A330970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n mod (1+d1), n mod (1+d2), ..., n mod (1+dk))
 * where d1 … dk are the decimal digits of n (most‑to‑least significant).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly to avoid empty digit list. */
    if (n == 0)
        return 0;

    /* Maximum decimal digits for 64‑bit n is 20. Allocate a fixed buffer. */
    uint8_t digits[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    seq_t result = 0;
    /* Build the result from most significant digit to least. */
    for (int i = len - 1; i >= 0; --i) {
        uint8_t d = digits[i];
        seq_t rem = (seq_t)n % ((seq_t)d + 1);
        result = result * 10 + rem;
    }
    return result;
}
