/*
 * A378999  Number of trailing 1-bits in the binary representation of
 *          sigma(n^2).
 * https://oeis.org/A378999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing 1-bits in binary representation of
 *            σ(n²)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 5325, u64 n <= 8505,
 *            u128 n <= 8505, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = v₂(1 + σ(n²)) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* For n ≥ 1 only, but safe to handle n == 0 */
    if (n == 0) return 0;

    /* Factorise n (simple trial division, n ≤ 10000 here) */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[32];
    unsigned  exps [32];
    size_t    cnt   = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            primes[cnt] = p;
            exps[cnt]   = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exps[cnt];
            }
            ++cnt;
        }
    }
    if (tmp > 1)
    {
        primes[cnt] = tmp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Compute σ(n²) = Π p^0 + p^1 + … + p^(2e) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t  p   = (seq_t)primes[i];
        seq_t  exp = (seq_t)(2 * exps[i] + 1);
        seq_t  geom = (big_pow(p, exp) - 1) / (p - 1);   /* Σ_{k=0}^{2e} p^k */
        sigma *= geom;
    }

    /* a(n) = v₂(1 + σ(n²))  -> count trailing zeros of σ+1 */
    seq_t val = sigma + 1;
    size_t v2 = 0;
    while ((val & (seq_t)1) == 0)
    {
        val >>= 1;
        ++v2;
    }
    return (seq_t)v2;
}
