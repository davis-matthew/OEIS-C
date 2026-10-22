/*
 * A365442  Partial sums of A365412.
 * https://oeis.org/A365442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{n} sigma(6*k + 2) where sigma(m) is the sum of
 *            all positive divisors of m. This implementation factors each
 *            number directly; it works for any seq_t type provided by the
 *            build environment and is valid C11
 * Range:     u8 n <= 6, u16 n <= 108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of divisors of an unsigned 64‑bit integer.
   The result is returned as type seq_t. */
static seq_t sigma_of_uint64(uint64_t m)
{
    seq_t sum = 1;
    uint64_t t = m;

    /* trial division using 2 separately, then odd candidates */
    if (t % 2 == 0) {
        seq_t term = 1;
        seq_t power = 1;
        do {
            power *= 2;
            term += power;
            t /= 2;
        } while (t % 2 == 0);
        sum *= term;
    }

    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            do {
                power *= p;
                term += power;
                t /= p;
            } while (t % p == 0);
            sum *= term;
        }
    }

    if (t > 1) {           /* remaining prime factor */
        sum *= (seq_t)(t + 1);
    }

    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t k = 0; k <= n; ++k) {
        uint64_t m = 6 * (uint64_t)k + 2;
        total += sigma_of_uint64(m);
    }

    return total;
}
