/*
 * A244604  Least number k such that k^n contains the digit n n times.
 * https://oeis.org/A244604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = least k such that the decimal representation of k^n
 *            contains the digit n exactly n times. For n ≥ 10 this is
 *            impossible, so a(n) = 0. For n = 0 a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_DIGITS 2000  /* sufficient for all n ≤ 9 and k in the range tested */

/* Counts digits of (base^exp) in decimal.  base is an unsigned
   64‑bit integer.  The count array must have 10 elements. */
static void digit_count_of_power(uint64_t base, idx_t exp, unsigned int count[10]) {
    uint8_t digits[MAX_DIGITS];
    size_t len = 0;
    uint64_t tmp = base;

    /* Convert base to decimal digits (least‑significant first) */
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp > 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    /* Multiply by base (exp-1) times */
    for (idx_t e = 1; e < exp; ++e) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t product = (uint64_t)digits[i] * base + carry;
            digits[i] = (uint8_t)(product % 10);
            carry = product / 10;
        }
        while (carry > 0) {
            digits[len++] = (uint8_t)(carry % 10);
            carry /= 10;
        }
    }

    /* Count digits */
    for (size_t i = 0; i < 10; ++i) count[i] = 0;
    for (size_t i = 0; i < len; ++i) ++count[digits[i]];
}

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n == 0) return (seq_t)1;
    if (n >= 10) return (seq_t)0;

    unsigned int freq[10];
    for (uint64_t k = 1; ; ++k) {
        digit_count_of_power(k, (idx_t)n, freq);
        if (freq[(unsigned int)n] == (unsigned int)n)
            return (seq_t)k;
    }
}
