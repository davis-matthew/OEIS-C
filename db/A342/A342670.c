/*
 * A342670  a(n) = gcd(n*sigma(A064989(n)), sigma(n)*A064989(n)), where
 *          A064989 is multiplicative with a(2^e) = 1 and a(p^e) =
 *          prevprime(p)^e for odd primes p, and sigma gives the sum of the
 *          divisors of its argument.
 * https://oeis.org/A342670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n * sigma(A064989(n)), sigma(n) * A064989(n)) where
 *            A064989 is multiplicative with A064989(2^e)=1 and
 *            A064989(p^e)=prevprime(p)^e for odd primes p. sigma(x) is the
 *            sum of the divisors of x
 * Range:     u8 n <= 18, u16 n <= 869, u32 n <= 8431, u64 n <= 8431,
 *            u128 n <= 8431, big any n
 * Helpers:   big_pow, factor_u64, gcd, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/prev_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Compute σ(k) for k <= 2^64-1 using multiplicative formula */
static seq_t sigma_u64(idx_t k)
{
    if (k == 0) return 0;
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64(k, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = exps[i];
        seq_t num = big_pow(p, (seq_t)(e + 1)) - 1;
        seq_t den = p - 1;
        seq_t sigma_p = num / den;          /* exact division */
        result *= sigma_p;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute A064989(n) */
    seq_t a064989 = 1;
    if (n > 1) {
        size_t cnt = 0;
        uint64_t primes[64], exps[64];
        factor_u64(n, primes, exps, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            if (primes[i] == 2) continue;    /* skip power of 2 */
            seq_t base = (seq_t)prev_prime(primes[i]);
            seq_t term = big_pow(base, (seq_t)exps[i]);
            a064989 *= term;
        }
    }

    /* Compute required σ values */
    seq_t sigma_a = sigma_u64((idx_t)a064989);
    seq_t sigma_n = sigma_u64(n);

    /* Compute the two terms whose gcd we need */
    seq_t term1 = (seq_t)n * sigma_a;
    seq_t term2 = sigma_n * a064989;

    /* Return the gcd */
    return gcd(term1, term2);
}
