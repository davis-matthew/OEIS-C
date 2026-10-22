/*
 * A327573  Partial sums of the number of infinitary divisors function: a(n) =
 *          Sum_{k=1..n} id(k), where id is A037445.
 * https://oeis.org/A327573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} id(k) where id is A037445 (infinitary divisor
 *            count)
 * Range:     u8 n <= 68, u16 n <= 8990, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_uint, pow2_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/popcount_uint.h"
#include "../utils/pow2_of.h"

static idx_t max_n = 0;          /* highest n for which data is ready */
static seq_t *cum = NULL;        /* cumulative sums a(k) */
static idx_t *spf = NULL;       /* smallest prime factor, spf[i] == 0 for i < 2 */

/* Extend precomputed tables up to n (if necessary). */
static void ensure_precompute(idx_t n)
{
    if (n <= max_n)
        return;

    size_t sz = (size_t)(n + 1);
    idx_t *new_spf = (idx_t *)realloc(spf, sz * sizeof(idx_t));
    seq_t *new_cum = (seq_t *)realloc(cum, sz * sizeof(seq_t));
    if (!new_spf || !new_cum)          /* allocation failed – keep old data */
        return;
    spf = new_spf;
    cum = new_cum;

    /* initialise spf array */
    for (size_t i = 0; i < sz; ++i)
        spf[i] = (idx_t)i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; (idx_t)((long double)i * (long double)i) <= n; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;

    /* make sure cum[0] = 0 */
    cum[0] = 0;

    /* compute id(k) for new k and cumulative sums */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        idx_t x = i;
        seq_t val = 1;          /* product of contributions for each prime */
        while (x > 1)
        {
            idx_t p = spf[x];
            int exp = 0;
            do
            {   /* divide out all copies of p */
                x /= p;
                ++exp;
            } while (x % p == 0);
            int pc = popcount_uint((unsigned int)exp);
            val *= pow2_of(pc);
        }
        cum[i] = cum[i - 1] + val;
    }
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but return 0 safely */
        return 0;
    ensure_precompute(n);
    return cum[(size_t)n];
}
