/*
 * A362414  a(n) = gcd(n, phi(n)^2) / gcd(n, phi(n)).
 * https://oeis.org/A362414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n,phi(n)^2)/gcd(n,phi(n))
 * Range:     u8 n <= 1781, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9429 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Greatest common divisor for seq_t values. */
static seq_t gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

seq_t a(idx_t n)
{
    /* Convert the index to the generic sequence type */
    seq_t n_seq = (seq_t)n;

    /* Euler's totient of n */
    seq_t phi_n = phi(n);

    /* First gcd */
    seq_t g = gcd(n_seq, phi_n);

    /* n divided by that gcd */
    seq_t n_div_g = n_seq / g;

    /* a(n) = gcd(n/g, g) */
    seq_t a_val = gcd(n_div_g, g);

    return a_val;
}
