/*
 * A242399  Write n and 3n in ternary representation and add all trits modulo
 *          3.
 * https://oeis.org/A242399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the result of digit‑wise addition modulo 3 of n and 3n in
 *            base 3, interpreted as a base‑3 number
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power  = 1;
    idx_t temp   = n;          /* copy of n for digit extraction */
    uint8_t prev_digit = 0;    /* digit of n shifted one place to the left */

    while (temp > 0 || prev_digit) {
        uint8_t digit     = (uint8_t)(temp % 3);      /* current trit of n */
        uint8_t added     = (digit + prev_digit) % 3; /* mod‑3 addition */
        result += (seq_t)added * power;              /* accumulate result */
        power  *= 3;                                 /* next power of 3   */
        temp   /= 3;                                 /* shift n right */
        prev_digit = digit;
    }
    return result;
}
