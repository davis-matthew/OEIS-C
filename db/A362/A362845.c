/*
 * A362845  Number of divisors of 7*n-2 of form 7*k+1.
 * https://oeis.org/A362845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A362845: Number of divisors of 7*n-2 that are ≡ 1 (mod 7).
 * Implementation uses prime factorization of 7*n-2 and a residue
 * dynamic programming to count divisors with residue 1 modulo 7.
 * The function returns the correct value for any n ≥ 1.
 */

#include <stddef.h>  /* for size_t */
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Compute m = 7*n - 2 (fits in 64‑bit for the intended range). */
    uint64_t m = 7ULL * (uint64_t)n - 2ULL;

    /* Factor m into prime powers. 64 slots suffice for all 64‑bit m. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* DP: cnt_resid[res] = number of divisors formed so far with residue 'res'. */
    seq_t resid[7] = {0};
    resid[1] = 1;                     /* divisor 1 has residue 1 */
    seq_t newres[7];

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        unsigned int pmod = (unsigned int)(p % 7ULL);

        /* reset newres */
        for (int k = 0; k < 7; ++k) newres[k] = 0;

        /* combine existing residues with powers of p */
        for (int r = 0; r < 7; ++r) {
            if (!resid[r]) continue;
            seq_t cur = resid[r];
            unsigned int mult = 1;          /* p^k mod 7, starting at k=0 */
            for (uint64_t k = 0; k <= e; ++k) {
                int newr = (int)((r * mult) % 7);
                newres[newr] += cur;
                mult = (unsigned int)((mult * pmod) % 7);
            }
        }
        /* copy newres back to resid */
        for (int k = 0; k < 7; ++k) resid[k] = newres[k];
    }

    /* The answer is the count of divisors ≡ 1 (mod 7). */
    return resid[1];
}
