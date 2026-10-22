/*
 * A369779  a(n) = n * Sum_{p|n, p prime} phi(n/p) / p.
 * https://oeis.org/A369779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p | n, p prime} (n/p) * phi(n/p) Equivalently: a(n)
 *            = n * Sum_{p | n, p prime} phi(n/p) / p
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = 0;

    /* factor n into distinct primes with exponents */
    uint64_t temp = nn;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            int e = 0;
            do {
                temp /= d;
                ++e;
            } while (temp % d == 0);
            primes[cnt] = d;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t sum = 0;

    /* for each distinct prime divisor p of n */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t m = nn / p;              /* m = n/p */

        /* compute phi(m) using the known factorization of n */
        seq_t phi = (seq_t)m;
        for (size_t j = 0; j < cnt; ++j) {
            int adjusted_exponent = exps[j] - (j == i ? 1 : 0);
            if (adjusted_exponent > 0) {
                phi = phi / primes[j] * (primes[j] - 1);
            }
        }

        seq_t term = (seq_t)m * phi;        /* m * phi(m) */
        sum += term;
    }

    return sum;
}
