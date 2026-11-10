/*
 * A344756  a(n) = A003415(n) / gcd(A003415(n), A069359(n)).
 * https://oeis.org/A344756
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) / gcd(A003415(n), A069359(n)), where A003415
 *            is the arithmetic derivative and A069359(n) = n*Sum_{p|n} 1/p
 * Range:     u8 n <= 95, u16 n <= 12012, u32 n <= 12012, u64 n <= 12012,
 *            u128 n <= 12012, big any n
 * Validated: standard, pass, 114 terms through n = 9987 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A003415_a(idx_t n) {
    if (n == 0 || n == 1)
        return 0;

    seq_t result = 0;
    seq_t original_n = (seq_t)n;
    seq_t remaining = original_n;

    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (original_n / p) * (seq_t)exponent;
        }
    }

    if (remaining > 1) {
        result += original_n / remaining;
    }

    return result;
}

static seq_t A069359_a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;
    idx_t m = n;

    if ((m & 1) == 0) {
        result += (seq_t)n / 2;
        while ((m & 1) == 0) m >>= 1;
    }

    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result += (seq_t)n / p;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1)
        result += (seq_t)n / m;

    return result;
}

static seq_t A344756_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t d = A003415_a(n);
    seq_t r = A069359_a(n);
    seq_t g = A344756_gcd(d, r);
    if (g == 0)
        return 0;
    return d / g;
}
