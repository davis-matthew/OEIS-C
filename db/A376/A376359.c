/*
 * A376359  Positions of numbers in A007961 that end in 2.
 * https://oeis.org/A376359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = position of the n-th number in A007961 whose last digit
 *            is 2 (search over m; last digit of A007961(m) is the greedy
 *            digit at place value 1^2)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A376359_isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

/* Last digit of A007961(m): the digit at place value 1. */
static idx_t A376359_last_digit(idx_t m) {
    idx_t k = A376359_isqrt(m);
    idx_t rem = m;
    idx_t digit = 0;
    while (k >= 1) {
        idx_t sq = k * k;
        idx_t max_digit = (sq + 2 * k) / sq;
        digit = rem / sq;
        if (digit > max_digit) digit = max_digit;
        rem -= digit * sq;
        k--;
    }
    return digit;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A376359_last_digit(m) == 2) count++;
    }
    return (seq_t)m;
}
