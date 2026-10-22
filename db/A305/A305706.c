/*
 * A305706  a(n) = smallest m such that the sum of digits of n^m is greater
 *          than n, or 0 if no such m exists.
 * https://oeis.org/A305706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m such that the sum of digits of n^m is greater
 *            than n (A007953(n^m) > n), or 0 if no such m exists
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* idx_t and seq_t are supplied by the build. */

seq_t a(idx_t n)
{
    /* Handle trivial cases. */
    if (n <= 1) {
        return 0;
    }

    /* Detect powers of 10 (including 0 and 1). */
    idx_t tmp = n;
    while ((tmp % 10) == 0) {
        tmp /= 10;
    }
    if (tmp == 1) {
        return 0;
    }

    /* Big integer represented as decimal digits (least‑significant first). */
    size_t capacity = 32;
    uint8_t *digits = (uint8_t *)malloc(capacity);
    if (!digits) {
        return 0;
    }

    size_t len = 0;
    idx_t val = n;
    while (val > 0) {
        if (len == capacity) {
            capacity <<= 1;
            uint8_t *tmp = (uint8_t *)realloc(digits, capacity);
            if (!tmp) {
                free(digits);
                return 0;
            }
            digits = tmp;
        }
        digits[len++] = (uint8_t)(val % 10);
        val /= 10;
    }
    if (len == 0) {        /* n == 0, but this is already handled above. */
        digits[0] = 0;
        len = 1;
    }

    /* Initial digit sum. */
    uint32_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        sum += digits[i];
    }

    for (idx_t m = 1; ; ++m) {
        if ((seq_t)sum > n) {
            free(digits);
            return (seq_t)m;
        }

        /* Multiply the current number by n. */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * (uint64_t)n + carry;
            digits[i] = (uint8_t)(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0) {
            if (len == capacity) {
                capacity <<= 1;
                uint8_t *tmp = (uint8_t *)realloc(digits, capacity);
                if (!tmp) {
                    free(digits);
                    return 0;
                }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }

        /* Recompute sum of digits. */
        sum = 0;
        for (size_t i = 0; i < len; ++i) {
            sum += digits[i];
        }
    }
}
