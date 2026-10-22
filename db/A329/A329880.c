/*
 * A329880  Numbers k such that the sums of unitary and nonunitary divisors of
 *          k have the same set of prime divisors.
 * https://oeis.org/A329880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 6715, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329880: numbers k such that rad(usigma(k)) = rad(nusigma(k)).
   The function iterates over integers k, finds those satisfying the
   property and returns the n-th such k.  It uses the helper
   factor_u64() to factor k, and rad() to compute the square‑free kernel
   of a number.  Candidates with nusigma(k)=0 are skipped. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/rad.h"         /* rad */

static bool is_candidate(uint64_t k)
{
    uint64_t primes[64], exps[64];
    size_t cnt;

    /* factor k */
    factor_u64(k, primes, exps, &cnt);

    __int128 sigma   = 1;   /* sigma(k)  */
    __int128 usigma  = 1;   /* usigma(k) */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute p^e */
        __int128 p_pow = 1;
        for (uint64_t j = 0; j < e; ++j)
            p_pow *= p;

        /* unitary part: (1 + p^e) for each prime factor */
        usigma *= (p_pow + 1);

        /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
        __int128 p_pow_e1 = p_pow * p;
        __int128 sigma_p = (p_pow_e1 - 1) / (p - 1);
        sigma *= sigma_p;
    }

    __int128 nusigma = sigma - usigma;
    if (nusigma <= 0)      /* nusigma(k) = 0 gives rad(0) unstable */
        return false;
    if (nusigma > (__int128)UINT64_MAX)
        return false;      /* cannot compute rad on larger values */

    /* cast to uint64_t for rad() */
    uint64_t sigma_u64   = (uint64_t)sigma;
    uint64_t usigma_u64  = (uint64_t)usigma;
    uint64_t nusigma_u64 = (uint64_t)nusigma;

    seq_t rad_us = rad((idx_t)usigma_u64);
    seq_t rad_nu = rad((idx_t)nusigma_u64);

    return rad_us == rad_nu;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n==0, return 0 for safety */

    uint64_t k = 1;
    idx_t count = 0;

    while (1)
    {
        if (is_candidate(k))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
