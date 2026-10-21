/*
 * A284256  a(n) = number of prime factors of n that are > the square of
 *          smallest prime factor of n (counted with multiplicity), a(1) = 0.
 * https://oeis.org/A284256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9944, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   factor_u64, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: a(n) = number of prime factors of n that are > (smallest prime factor of n)^2
   (counted with multiplicity). The sequence starts at a(1) = 0. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/lpf.h"          /* lpf(idx_t n) → smallest prime factor of n */
#include "../utils/factor_u64.h"    /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0.
       For n < 2 we return 0, which covers a(0) and any calls with n = 0. */
    if (n < 2)
        return 0;

    uint64_t x   = (uint64_t)n;
    uint64_t spf = lpf(x);                /* smallest prime factor of x */

    /* Compute spf^2 in 128‑bit to avoid overflow. */
    unsigned __int128 spf_sq = (unsigned __int128)spf * spf;

    /* Factorize x; arrays are sized large enough to hold all prime factors. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t   cnt;
    factor_u64(x, primes, exps, &cnt);

    /* Count multiplicities of primes that are strictly greater than spf^2. */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if ((unsigned __int128)primes[i] > spf_sq) {
            res += (seq_t)exps[i];
        }
    }

    return res;
}
