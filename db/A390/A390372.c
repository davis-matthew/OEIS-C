/*
 * A390372  The sum of the bi-unitary divisors of n that are exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A390372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with * a(p^e) = 1 +
 *            (p^{2*floor((e-1)/2)+2} - 1) * p / (p^2 - 1) * - [ e ≡ 2 (mod 4)
 *            ] * p^{e/2} * (where [] is the Iverson bracket). * For n = 1,
 *            a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 3612, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"
#include "../utils/factor_u64.h"

/* Compute the local factor a(p^e) */
static seq_t local_factor(uint64_t p, uint64_t e)
{
    seq_t result = 1;

    /* exp1 = 2*floor((e-1)/2) + 2 */
    uint64_t t = (e - 1) / 2;
    int exp1 = (int)(2 * t + 2);

    seq_t p_seq    = (seq_t)p;
    seq_t p_pow    = pow_seq(p_seq, exp1);
    seq_t denom    = p_seq * p_seq - 1;
    seq_t numerator = (p_pow - 1) * p_seq;
    seq_t term     = numerator / denom;

    result += term;

    if (e % 4 == 2) {
        int half = (int)(e / 2);
        seq_t p_half = pow_seq(p_seq, half);
        result -= p_half;
    }

    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    /* factor n; assume factor_u64 writes primes and exponents into arrays */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= local_factor(primes[i], exps[i]);
    }
    return prod;
}
