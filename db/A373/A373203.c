/*
 * A373203  a(n) = minimum k>1 such that n^k contains all distinct decimal
 *          digits of n.
 * https://oeis.org/A373203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimum k>1 such that the decimal representation of n^k
 *            contains all distinct decimal digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_have_all
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digits_have_all.h"

seq_t a(idx_t n)
{
    /* Convert n to a type small enough for multiplication */
    uint32_t n_val = (uint32_t)n;

    /* Extract the decimal digits of n and build its digit mask */
    uint32_t base_mask = 0;
    uint8_t  base_digits[20];
    size_t   base_len = 0;

    uint32_t tmp = n_val;
    if (tmp == 0) {
        base_digits[base_len++] = 0;
        base_mask |= 1u << 0;
    } else {
        while (tmp > 0) {
            uint32_t d = tmp % 10;
            base_digits[base_len++] = (uint8_t)d;
            base_mask |= 1u << d;
            tmp /= 10;
        }
    }

    /* power_digits will hold the current value of n^k in little‑endian
       decimal representation (digits[0] is units). */
    uint8_t  power_digits[300];
    size_t   power_len = 0;

    /* Start with n^1 */
    for (size_t i = 0; i < base_len; ++i)
        power_digits[i] = base_digits[i];
    power_len = base_len;

    /* Search for the minimal k>1 */
    for (idx_t k = 2; ; ++k) {
        /* Multiply current value by n (still small enough for 32‑bit ops). */
        uint32_t carry = 0;
        for (size_t i = 0; i < power_len; ++i) {
            uint32_t prod = (uint32_t)power_digits[i] * n_val + carry;
            power_digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            power_digits[power_len++] = carry % 10;
            carry /= 10;
        }

        /* Check whether all digits of n are present in the current power. */
        if (digits_have_all(power_digits, power_len, base_mask))
            return (seq_t)k;
    }
}
