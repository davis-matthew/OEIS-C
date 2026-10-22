/*
 * A327171  a(n) = phi(n) * core(n), where phi is Euler totient function, and
 *          core gives the squarefree part of n.
 * https://oeis.org/A327171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^k∥n} (p-1) · p^{(k-1)+(k mod 2)} (i.e. a(n) = φ(n) *
 *            core(n) where core(n) = ∏_{p|n, k odd} p)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9847, u64 n <= 9847,
 *            u128 n <= 9847, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle n = 1 separately (product over an empty set is 1). */
    if (n == 1) return (seq_t)1;

    /* Prime factorisation of n.  For n ≤ 2^64, <64 distinct primes are enough. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* p   = primes[i]      (uint64_t)
           k   = exps[i]        (uint64_t) */
        uint64_t p = primes[i];
        uint64_t k = exps[i];

        /* φ(p^k) = p^{k-1} * (p-1)  (k ≥ 1) */
        seq_t phi_part = (seq_t)(p - 1);
        if (k > 1) {
            seq_t pow_part = big_pow((seq_t)p, (seq_t)(k - 1));
            phi_part *= pow_part;
        }

        /* Core contribution: multiply by p iff k is odd. */
        if (k & 1ULL) {
            phi_part *= (seq_t)p;
        }

        result *= phi_part;
    }

    return result;
}
