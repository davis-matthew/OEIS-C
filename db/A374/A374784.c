/*
 * A374784  Denominator of the mean unitary abundancy index of the unitary
 *          divisors of n.
 * https://oeis.org/A374784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of the mean unitary abundancy index of the
 *            unitary divisors of n = denominator of ∏_{p^a || n} (2 p^a +
 *            1)/(2 p^a) after reducing the fraction
 * Range:     u8 n <= 29, u16 n <= 4181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    /* Factor n: primes and exponents. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* k = number of distinct primes. */
    int k = (int)cnt;

    /* Initial denominator = 2^k * n. */
    seq_t denom = (seq_t)n;
    if (k > 0) {
        denom *= pow_seq((seq_t)2, k);
    }

    /* Numerator = product over primes of (2 p^a + 1). */
    seq_t numer = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_pow = pow_seq((seq_t)primes[i], (int)exps[i]); /* p^a */
        seq_t term = p_pow * (seq_t)2 + 1;                     /* 2 p^a + 1 */
        numer *= term;
    }

    /* Reduce the fraction and return denominator. */
    seq_t g = gcd(numer, denom);
    denom /= g;
    return denom;
}
