/*
 * A330633  The concatenation of the products of every pair of consecutive
 *          digits of n (with a(n) = 0 for 0 <= n <= 9).
 * https://oeis.org/A330633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = integer obtained by concatenating the decimal
 *            representations of adjacent‑digit products of n; a(n)=0 for n<10
 * Range:     u8 n <= 133, u16 n <= 1724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 10) return 0;
    /* Extract decimal digits of n into an array in MSB→LSB order. */
    uint8_t digits[20];           /* 64‑bit n fits in at most 20 decimal digits */
    int len = 0;
    while (n > 0) {
        digits[len++] = n % 10;
        n /= 10;
    }
    /* Reverse to get most significant digit first. */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }
    /* Concatenate the products of consecutive digits. */
    seq_t result = 0;
    for (int i = 0; i < len - 1; ++i) {
        uint8_t prod = digits[i] * digits[i + 1];
        seq_t mul = (prod < 10) ? 10 : 100;   /* 1‑digit or 2‑digit product */
        result = result * mul + (seq_t)prod;
    }
    return result;
}
