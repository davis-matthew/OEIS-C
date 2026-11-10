/*
 * A376358  Positions of numbers in A007961 that end in 1.
 * https://oeis.org/A376358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 such that A007961(m) ends in digit 1; found
 *            by scanning m and running the greedy square-base expansion (only
 *            the last digit is tracked)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A376358_isqrt(idx_t n) {
    if (n == 0) return 0;
    idx_t x = n;
    idx_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

static idx_t A376358_lastdigit(idx_t m) {
    idx_t k = A376358_isqrt(m);
    idx_t rem = m;
    idx_t digit = 0;
    while (k >= 1) {
        idx_t sq = k * k;
        idx_t nsq = (k + 1) * (k + 1);
        idx_t maxd = (nsq - 1) / sq;
        digit = rem / sq;
        if (digit > maxd) digit = maxd;
        rem -= digit * sq;
        k--;
    }
    return digit % 10;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A376358_lastdigit(m) == 1) count++;
    }
    return (seq_t)m;
}
