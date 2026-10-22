/*
 * A335054  Infinitary barely abundant numbers: infinitary abundant numbers
 *          whose infinitary abundancy is closer to 2 than that of any smaller
 *          infinitary abundant number.
 * https://oeis.org/A335054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 203, u32 n <= 677, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Infinitary barely abundant numbers (OEIS A335054)
 *
 * This function generates the sequence of integers that are
 * infinitary abundant and whose infinitary abundancy ratio
 * σ_inf(n)/n is strictly closer to 2 than any smaller such
 * integer.  The infinitary divisor sum σ_inf(n) is computed
 * from the prime factorisation of n:
 *
 *   σ_inf(p^e) = ∏_{k : bit k of e is 1} (1 + p^(2^k))
 *   σ_inf(n)   = ∏ σ_inf(p_i^{e_i})  for n = ∏ p_i^{e_i}
 *
 * A number n is an infinitary barely abundant number if it is
 * infinitary abundant (σ_inf(n) > 2 n) and
 *
 *   |σ_inf(n) / n – 2|  <  |σ_inf(m) / m – 2|
 *
 * holds for every infinitary abundant integer m < n.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* helper: compute σ_inf(n) and whether it is infinitary abundant */
static seq_t calc_sigma_inf(uint64_t n, bool *abundant)
{
    /* arrays large enough for any 64‑bit number */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p   = pr[i];
        unsigned e  = (unsigned)ex[i];
        unsigned bit = 0;
        unsigned tmp = e;
        while (tmp)
        {
            if (tmp & 1U)
            {
                seq_t pw = pow_uint(p, 1ULL << bit);
                sigma *= (pw + 1);
            }
            tmp >>= 1;
            ++bit;
        }
    }

    *abundant = (sigma > (seq_t)2 * (seq_t)n);
    return sigma;
}

/* dynamic cache of found terms */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache = seq_alloc_new_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* best difference information */
static seq_t best_num = 0;  /* numerator |σ_inf - 2n| of best so far */
static seq_t best_den = 1;  /* denominator n of best so far */
static bool best_set   = false;

/* next integer to test */
static uint64_t next_candidate = 1;

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Grow the cache to contain at least n terms */
    while (cache_len < (size_t)n)
    {
        uint64_t m = next_candidate;
        bool abundant = false;
        seq_t sigma = calc_sigma_inf(m, &abundant);

        if (abundant)
        {
            seq_t diff_num = sigma - (seq_t)2 * (seq_t)m;
            seq_t diff_den = (seq_t)m;

            bool better;
            if (!best_set)   /* first abundance found */
                better = true;
            else
                better = (diff_num * best_den < best_num * diff_den);

            if (better)
            {
                best_num = diff_num;
                best_den = diff_den;
                best_set = true;

                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)m;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
