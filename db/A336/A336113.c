/*
 * A336113  a(n) is the numerator of Sum_{odd d|n} 1/d.
 * https://oeis.org/A336113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum of reciprocals of odd divisors of n
 * Range:     u8 n <= 16, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Extract the odd part of n */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0ULL)
        odd >>= 1;

    /* Odd part 1 gives the rational 1, numerator 1 */
    if (odd == 1ULL)
        return (seq_t)1;

    /* Factor the odd part */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(odd, primes, exps, &cnt);

    /* Compute \sigma(odd) = product_{i} (p_i^{a_i+1} - 1)/(p_i - 1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)1;
        /* Compute p^{a+1} */
        seq_t p_pow = (seq_t)1;
        for (uint64_t k = 0; k < exps[i] + 1; ++k)
            p_pow *= (seq_t)primes[i];
        term = (p_pow - 1) / ((seq_t)primes[i] - 1);
        sigma *= term;
    }

    /* Reduce sigma / odd to lowest terms */
    seq_t odd_seq = (seq_t)odd;
    seq_t g = gcd(sigma, odd_seq);
    return sigma / g;
}
