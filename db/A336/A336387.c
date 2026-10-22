/*
 * A336387  Number of prime divisors of n that do not divide sigma(n); a(1) =
 *          0.
 * https://oeis.org/A336387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p, distinct prime divisor of n} [σ(n) mod p ≠ 0]
 * Range:     u8 n <= 22, u16 n <= 5685, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function factor_u64 for prime factorisation. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (uint64_t)1) result *= base;
        base *= base;
        exp >>= (uint64_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;

    /* Factorise n */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ(n) = Π over p^e of (p^(e+1)-1)/(p-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t numerator = pow_seq((seq_t)p, e + (uint64_t)1); /* p^(e+1) */
        seq_t term = (numerator - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }

    /* Count primes p such that σ(n) mod p != 0 */
    seq_t count = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if ((sigma % (seq_t)p) != (seq_t)0) ++count;
    }

    return count;
}
