/*
 * A327526  Maximum uniform divisor of n.
 * https://oeis.org/A327526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum uniform divisor of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number is uniform if all prime exponents are equal, i.e. it is a power
   of a squarefree number.  For n = ∏ p_i^{a_i}, the maximum uniform
   divisor is max_{e≥1} ∏{p_i | a_i ≥ e} p_i^e. */

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* handle n = 1 */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* factor n.  For u64 the number of distinct primes is small (<64). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt == 0) {
        /* n is 1, handled above, but keep for safety */
        return (seq_t)1;
    }

    /* find the maximum exponent among the prime factors */
    uint64_t maxExp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > maxExp) {
            maxExp = exps[i];
        }
    }

    seq_t best = (seq_t)0;            /* 0 will indicate not set yet */

    for (uint64_t e = 1; e <= maxExp; ++e) {
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] >= e) {
                seq_t pw = big_pow((seq_t)primes[i], (seq_t)e);
                prod *= pw;
            }
        }
        if (best == (seq_t)0 || prod > best) {
            best = prod;
        }
    }

    return best;
}
