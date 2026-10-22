/*
 * A346403  a(1)=1; for n>1, a(n) gives the sum of the exponents in the
 *          different ways to write n as n = x^y, 2 <= x, 1 <= y.
 * https://oeis.org/A346403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the exponents y in all representations n = x^y (x
 *            ≥ 2, y ≥ 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = σ(g), where g = gcd of the prime‑exponents of n */
#include <stdint.h>
#include <stdbool.h>

/* Compute gcd of two uint64_t arguments */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;            /* a(1) = 1 by definition */

    idx_t tmp = n;
    idx_t g = 0;                    /* gcd of exponents */

    /* Factor 2 separately */
    if ((tmp & 1) == 0) {
        int exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            exp++;
        }
        g = (g == 0 ? (idx_t)exp : gcd_idx(g, (idx_t)exp));
    }

    /* Factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            g = (g == 0 ? (idx_t)exp : gcd_idx(g, (idx_t)exp));
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (tmp > 1) {
        g = (g == 0 ? (idx_t)1 : (g == 1 ? (idx_t)1 : (idx_t)1));
    }

    /* Compute σ(g) – sum of divisors of g */
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            sum += (seq_t)d;
            if (d != g / d)
                sum += (seq_t)(g / d);
        }
    }
    return sum;
}
