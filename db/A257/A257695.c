/*
 * A257695  a(0) = 1; for n >= 1, a(n) = lcm(A060130(n), a(A257684(n))).
 * https://oeis.org/A257695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = lcm(A060130(n), a(A257684(n)))
 * Range:     u8 n <= 8518, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 137 terms through n = 9360 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A257695_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

/* A060130: number of nonzero digits in factorial base */
static seq_t A257695_nz(idx_t n)
{
    seq_t count = 0;
    idx_t i = 1;
    while (n > 0) {
        if (n % (i + 1) != 0) ++count;
        n /= (i + 1);
        ++i;
    }
    return count;
}

/* A257684: drop the rightmost factorial-base digit, decrement nonzero digits */
static idx_t A257695_next(idx_t n)
{
    idx_t k = n;
    idx_t i = 1;
    idx_t res = 0;
    idx_t w = 1;            /* (i-1)! for i >= 2 */
    while (k > 0) {
        idx_t d = k % (i + 1);
        k /= (i + 1);
        if (i >= 2) {
            if (d > 0) d--;
            res += d * w;
            w *= i;
        }
        ++i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    while (n > 0) {
        seq_t c = A257695_nz(n);
        seq_t g = A257695_gcd(result, c);
        result = result / g * c;
        n = A257695_next(n);
    }
    return result;
}
