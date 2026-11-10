/*
 * A257696  a(0) = 0; for n >= 1, a(n) = gcd(A060130(n), a(A257684(n))).
 * https://oeis.org/A257696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = gcd(A060130(n), a(A257684(n))) for n >= 1
 * Range:     u8 n <= 10080, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 137 terms through n = 8825 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A257696_gcd(idx_t x, idx_t y)
{
    while (y) { idx_t t = x % y; x = y; y = t; }
    return x;
}

/* A060130: number of nonzero digits in factorial base */
static idx_t A257696_nz(idx_t n)
{
    idx_t count = 0, i = 1;
    while (n > 0) {
        if (n % (i + 1) != 0) ++count;
        n /= (i + 1);
        ++i;
    }
    return count;
}

/* A257684: drop last factorial digit, decrement nonzero remaining digits */
static idx_t A257696_next(idx_t n)
{
    idx_t fact[21];
    unsigned digits[21];
    int i;
    idx_t m = n, result = 0, factor = 1;
    fact[0] = 1;
    for (i = 1; i <= 20; ++i) fact[i] = fact[i - 1] * (idx_t)i;
    for (i = 0; i <= 20; ++i) digits[i] = 0;
    for (i = 20; i >= 1; --i) {
        digits[i] = (unsigned)(m / fact[i]);
        m %= fact[i];
    }
    for (i = 2; i <= 20; ++i) {
        idx_t d = (idx_t)digits[i];
        if (d > 0) --d;
        result += d * factor;
        factor *= (idx_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t chain[128];
    int len = 0, k;
    idx_t val = 0;
    while (n > 0 && len < 128) {
        chain[len++] = n;
        n = A257696_next(n);
    }
    for (k = len - 1; k >= 0; --k) {
        val = A257696_gcd(A257696_nz(chain[k]), val);
    }
    return (seq_t)val;
}
