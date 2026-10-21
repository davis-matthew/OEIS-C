/*
 * A284255  Largest divisor of n such that all its prime factors are less than
 *          the square of the smallest prime factor of n, a(1) = 1.
 * https://oeis.org/A284255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor of n such that every prime factor of the
 *            divisor is strictly less than (smallest prime factor of n)^2
 * Range:     u8 n <= 255, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
/* No external headers required. */

seq_t a(idx_t n) {
    if (n == 1u) return (seq_t)1;

    /* Work with 64‑bit integers for the factorisation. */
    uint64_t nn = (uint64_t)n;

    /* Simple trial division factorisation. */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt = 0;
    uint64_t t   = nn;

    for (uint64_t d = 2; d * d <= t; ++d) {
        if (t % d == 0) {
            int e = 0;
            while (t % d == 0) {
                t   /= d;
                e++;
            }
            primes[cnt] = d;
            exps[cnt]   = e;
            cnt++;
        }
    }
    if (t > 1) {
        primes[cnt] = t;
        exps[cnt]   = 1;
        cnt++;
    }

    /* Smallest prime factor is primes[0] because we processed d in increasing order. */
    uint64_t p_min = primes[0];
    uint64_t threshold = p_min * p_min;        /* fits in 64 bits for the limits used */

    /* Construct the largest divisor with allowed prime factors. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] < threshold) {
            /* multiply result by primes[i] raised to exps[i] */
            for (int e = 0; e < exps[i]; ++e)
                result *= (seq_t)primes[i];
        }
    }
    return result;
}
