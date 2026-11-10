/*
 * A242400  Differences between A008586 (multiples of 4) and A242399.
 * https://oeis.org/A242400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n - A242399(n), where A242399(n) is the digit-wise sum
 *            mod 3 of n and 3n in base 3
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9575 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A242399_a(idx_t n) {
    seq_t result = 0;
    seq_t power  = 1;
    idx_t temp   = n;
    uint8_t prev_digit = 0;

    while (temp > 0 || prev_digit) {
        uint8_t digit = (uint8_t)(temp % 3);
        uint8_t added = (uint8_t)((digit + prev_digit) % 3);
        result += (seq_t)added * power;
        power  *= 3;
        temp   /= 3;
        prev_digit = digit;
    }
    return result;
}

seq_t a(idx_t n)
{
    return (seq_t)n * 4 - A242399_a(n);
}
