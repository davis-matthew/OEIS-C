/*
 * A324502  a(n) = denominator of Sum_{d|n} (1/pod(d)) where pod(k) = the
 *          product of the divisors of k (A007955).
 * https://oeis.org/A324502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = lcm{ pod(d) | d divides n } where pod(d) = product of
 *            divisors of d
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes pod(d) = d^{τ(d)/2} and takes the lcm over the divisors of n. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Maximum distinct prime factors for reasonable n (e.g., n <= 10^10) */
#define MAX_PRF 64

/* Helper that enumerates all divisors of n, updating best pod exponents. */
static void enumerate_divisors(const uint64_t *ex, size_t cnt, uint64_t *f,
                               uint64_t t_sofar, uint64_t *best)
{
    if (cnt == 0) {
        /* Only n=1 case: no primes, t_sofar == 1 already. Nothing to update. */
        return;
    }
    if (cnt == 0) {
        uint64_t tau = t_sofar;
        /* No primes, nothing to update */
        return;
    }
    /* Recursive enumeration: either write a recursive function manually or use loop with stack. */
    /* We'll implement recursion directly via index parameter. */
    /* But since C doesn't have nested functions, we write an external static helper. */
}

/* Recursive worker for divisor enumeration */
static void rec_div(size_t pos, size_t cnt,
                    const uint64_t *ex, uint64_t *f,
                    uint64_t t_sofar, uint64_t *best)
{
    if (pos == cnt) {
        uint64_t tau = t_sofar;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t pexp = f[i] * tau / 2;
            if (pexp > best[i]) best[i] = pexp;
        }
        return;
    }
    for (uint64_t v = 0; v <= ex[pos]; ++v) {
        f[pos] = v;
        uint64_t new_t = t_sofar * (v + 1);
        rec_div(pos + 1, cnt, ex, f, new_t, best);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */

    /* Factorize n */
    uint64_t primes[MAX_PRF], exps[MAX_PRF];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Arrays to hold best pod exponents per prime */
    uint64_t best[MAX_PRF] = {0};
    uint64_t f[MAX_PRF] = {0};

    if (cnt == 0) {
        /* n == 1 */
        return (seq_t)1;
    }

    /* Enumerate all divisors */
    rec_div(0, cnt, exps, f, (uint64_t)1, best);

    /* Compute result as product of primes^best[p] */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pow_p = big_pow((seq_t)primes[i], (seq_t)best[i]);
        result *= pow_p;
    }

    return result;
}
