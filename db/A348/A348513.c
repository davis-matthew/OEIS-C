/*
 * A348513  Möbius transform of A048146, the sum of non-unitary divisors of n.
 * https://oeis.org/A348513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Sum_{m|n} μ(n/m) * ∏_{p|m} (1 + p^{v_p(m)})
 * Range:     u8 n <= 395, u16 n <= 9666, u32 n <= 9666, u64 n <= 9666,
 *            u128 n <= 9666, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the Möbius transform of A048146 (sum of non‑unitary divisors). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void dfs(const uint64_t *pr, const uint64_t *ex, size_t pcnt,
                size_t idx, idx_t cur_m, seq_t U, int mu,
                seq_t *sum)
{
    if (idx == pcnt) {
        if (mu == 1)  *sum += U;
        else if (mu == -1) *sum -= U;
        return;
    }

    uint64_t prime = pr[idx];
    uint64_t emax  = ex[idx];
    idx_t    m     = cur_m;
    idx_t    p_pow = 1;

    for (uint64_t e = 0; e <= emax; ++e) {
        /* contribution of this prime to μ(n/m) */
        uint64_t rem = emax - e;          /* exponent of this prime in n/m */
        int factor;
        if (rem == 0)      factor = 1;
        else if (rem == 1) factor = -1;
        else               factor = 0;
        int new_mu;
        if (factor == 0)
            new_mu = 0;
        else if (factor == 1)
            new_mu = mu;
        else
            new_mu = -mu;

        /* product over primes in m of (1 + p^e) */
        seq_t U_new = U;
        if (e > 0)
            U_new = U * ((seq_t)1 + (seq_t)p_pow);

        /* recurse for the rest of the primes */
        idx_t m_next = cur_m * p_pow;      /* cur_m * p^e */
        dfs(pr, ex, pcnt, idx + 1, m_next, U_new, new_mu, sum);

        /* prepare p^ (e+1) for next iteration */
        if (e < emax)
            p_pow *= prime;
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  pcnt = 0;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t sumS2 = 0;
    dfs(primes, exps, pcnt, 0, (idx_t)1, (seq_t)1, 1, &sumS2);

    seq_t result = (seq_t)n - sumS2;
    return result;
}
