/*
 * A318827  a(n) = n - gcd(n - 1, phi(n)).
 * https://oeis.org/A318827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n-1, phi(n))
 * Range:     u8 n <= 257, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9978 (2026-11-01)
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
    /* n ≥ 1 by definition of the sequence */
    seq_t phi_n  = phi(n);            /* φ(n) */
    seq_t n_minus1 = (seq_t)n - 1;    /* n-1 as seq_t */
    seq_t g = gcd(n_minus1, phi_n);   /* gcd(n-1, φ(n)) */
    return (seq_t)n - g;              /* a(n) = n - g */
}
