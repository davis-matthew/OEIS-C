/*
 * A375523  a(n) = numerator of Sum_{i=1..n} 1/A171397(i).
 * https://oeis.org/A375523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Sum_{i=1..n} 1/A171397(i), where A171397(i)
 *            reads the decimal digits of i as a base-11 number
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1974 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A171397_a(idx_t n)
{
    seq_t res = 0;
    seq_t power = 1;
    idx_t t = n;
    while (t != 0) {
        seq_t digit = (seq_t)(t % 10);
        res += digit * power;
        power *= 11;
        t /= 10;
    }
    return res;
}

static seq_t A375523_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t num = 0, den = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t d = A171397_a(i);
        seq_t g = A375523_gcd(den % d, d);   /* gcd(den, d) */
        seq_t dg = d / g;
        num = num * dg + den / g;
        den = den * dg;
        /* any common factor of num and den divides d */
        for (;;) {
            seq_t h = A375523_gcd(num % d, d);
            if (h == 1) break;
            h = A375523_gcd(h, den % h == 0 ? h : A375523_gcd(den % d, h));
            if (h <= 1) break;
            num /= h;
            den /= h;
        }
    }
    return num;
}
