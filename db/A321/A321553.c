/*
 * A321553  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^8.
 * https://oeis.org/A321553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Helper recursive function that enumerates all divisors of n */
static void dfs_div(int idx, seq_t cur, const uint64_t *pr, const uint64_t *ex, size_t cnt,
                    seq_t *pos, seq_t *neg, uint64_t n)
{
    if ((size_t)idx == cnt) {
        seq_t d = cur;
        uint64_t over = n / (uint64_t)d;          /* n / d  (fits in uint64_t) */
        seq_t pow8 = (seq_t)1;
        for (int i = 0; i < 8; ++i) pow8 *= d;    /* d^8 */

        if (((over + 1ULL) & 1ULL) == 0ULL)
            *pos += pow8;                         /* (+) sign */
        else
            *neg += pow8;                         /* (-) sign */
        return;
    }

    uint64_t p = pr[idx];
    uint64_t emax = ex[idx];
    seq_t cur_pow = (seq_t)1;
    for (uint64_t e = 0; e <= emax; ++e) {
        dfs_div(idx + 1, cur * cur_pow, pr, ex, cnt, pos, neg, n);
        cur_pow *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)  /* n=0 is outside of the OEIS offset but defined safely */
        return (seq_t)0;

    /* Factorise n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t pos = 0, neg = 0;
    dfs_div(0, (seq_t)1, primes, exps, cnt, &pos, &neg, (uint64_t)n);

    /* The sequence is always positive for n≥1, so subtraction is safe */
    return pos - neg;
}
