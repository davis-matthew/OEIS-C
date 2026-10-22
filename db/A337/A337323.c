/*
 * A337323  a(n) = gcd(n, tau(n), sigma(n), pod(n)) where tau(k) is the number
 *          of divisors of k (A000005), sigma(k) is the sum of divisors of k
 *          (A000203) and pod(k) is the product of divisors of k (A007955).
 * https://oeis.org/A337323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, tau(n), sigma(n)) (pod is redundant)
 * Range:     u8 n <= 37, u16 n <= 3661, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Return the OEIS sequence A337323 value for index n (n>=1). */
seq_t a(idx_t n)
{
    if (n == 0)  /* defensive guard; the sequence is defined for n>=1 */
        return 0;

    seq_t n_seq = (seq_t)n;        /* promote the index to seq_t */

    /* factor n using the provided helper. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute tau(n) and sigma(n). */
    seq_t tau   = 1;
    seq_t sigma = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* tau *= (e+1) */
        tau *= (seq_t)(e + 1);

        /* sigma *= (p^(e+1)-1)/(p-1) */
        seq_t pow = big_pow((seq_t)p, (seq_t)(e + 1));
        seq_t part = (pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= part;
    }

    /* GCD of n, tau, sigma. */
    seq_t g = gcd(n_seq, tau);
    g = gcd(g, sigma);

    return g;
}
