/*
 * A348271  a(n) is the sum of noninfinitary divisors of n.
 * https://oeis.org/A348271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(n) - A049417(n)
 * Range:     u8 n <= 16, u16 n <= 682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sum of noninfinitary divisors of n (offset 1). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Sum of noninfinitary divisors of n. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma     = (seq_t)1; /* Σ d | n                         */
    seq_t sigma_inf = (seq_t)1; /* Σ of infinitary divisors of n  */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* σ(p^e) = (p^{e+1} - 1) / (p - 1) */
        seq_t sigma_pe = (seq_pow(p, (idx_t)(e + 1)) - (seq_t)1) /
                         ((seq_t)p - (seq_t)1);

        /* σ_inf(p^e) = ∏_{bits j of e} (1 + p^{2^j}) */
        seq_t sigma_inf_pe = (seq_t)1;
        for (uint64_t j = 0; e; ++j, e >>= 1) {
            if (e & 1ULL) {
                seq_t term = (seq_t)1 + seq_pow(p, (idx_t)(1ULL << j));
                sigma_inf_pe *= term;
            }
        }

        sigma     *= sigma_pe;
        sigma_inf *= sigma_inf_pe;
    }

    return sigma - sigma_inf;
}
