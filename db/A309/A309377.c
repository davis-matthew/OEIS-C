/*
 * A309377  a(n) is the product of the divisors of n^n (A000312).
 * https://oeis.org/A309377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of divisors of n^n = (n^n)^{τ(n^n)/2}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   _smallest_prime_factor, big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_smallest_prime_factor.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Prime factorisation of n */
    uint64_t rem = n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    while (rem > 1) {
        uint64_t p = _smallest_prime_factor(rem);
        uint64_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }
        primes[cnt]   = p;
        exps[cnt]     = e;
        ++cnt;
    }

    /* Compute τ(n^n) = ∏ (e_i * n + 1) */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t a_i = (seq_t)exps[i] * (seq_t)n;   /* exponent of p_i in n^n */
        tau = seq_mul(tau, a_i + (seq_t)1);
    }

    /* Compute product of divisors: ∏ p_i^{(e_i * n * τ)/2} */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t a_i = (seq_t)exps[i] * (seq_t)n;
        seq_t exp = seq_mul(a_i, tau);
        exp = exp / 2;                        /* integer division is exact */
        seq_t pw  = big_pow((seq_t)primes[i], exp);
        result = seq_mul(result, pw);
    }

    return result;
}
