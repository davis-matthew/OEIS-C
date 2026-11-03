/*
 * A354618  a(n) = (sum of the digits of 5^n) - (sum of the digits of 2^n).
 * https://oeis.org/A354618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum of digits of 5^n) - (sum of digits of 2^n)
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 898 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t digit_sum_power(unsigned base, idx_t exp)
{
    /* Compute digit sum of base^exp using a decimal digit array. */
    size_t cap = 64;
    uint8_t *dig = (uint8_t *)malloc(cap);
    if (!dig) return 0;          /* allocation failed – not expected in tests */
    size_t len = 1;
    dig[0] = 1;                  /* start with number 1 */

    for (idx_t i = 0; i < exp; ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint32_t v = dig[j] * base + carry;
            dig[j] = (uint8_t)(v % 10);
            carry = v / 10;
        }
        while (carry) {
            if (len == cap) {
                cap <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(dig, cap);
                if (!tmp) { free(dig); return 0; } /* allocation failed */
                dig = tmp;
            }
            dig[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    seq_t sum = 0;
    for (size_t j = 0; j < len; ++j)
        sum += dig[j];
    free(dig);
    return sum;
}

seq_t a(idx_t n)
{
    seq_t s5 = digit_sum_power(5, n);
    seq_t s2 = digit_sum_power(2, n);
    return s5 - s2;
}
