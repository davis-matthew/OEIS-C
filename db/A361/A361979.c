/*
 * A361979  Expansion of 1 / Sum_{k>=0} x^(k*(2*k - 1)).
 * https://oeis.org/A361979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 For n ≥ 1: a(n) = –∑_{k≥1 , k(2k−1)≤n} a(n – k(2k–1))
 * Range:     i8 n <= 27, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uncomment the following line if compiling as C++ to silence
   conversion warnings.  It is harmless for C.  */
#ifndef __cplusplus
#define seq_cast(x) ((seq_t*)(x))
#else
#define seq_cast(x) reinterpret_cast<seq_t*>(x)
#endif

#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache   = NULL;
static idx_t  cache_len = 0U; /* number of terms stored (next index to compute) */
static idx_t  cache_cap = 0U; /* allocated capacity */

/* grow cache to include at least 'needed' terms  */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    /* grow capacity: double until enough */
    idx_t new_cap = cache_cap == 0U ? 64U : cache_cap;
    while (new_cap <= needed)
        new_cap <<= 1U;
    /* realloc and cast for both C and C++ */
    seq_t *new_cache = seq_cast(realloc(cache, (size_t)new_cap * sizeof(seq_t)));
    if (!new_cache)
        abort();          /* fatal: out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* ensure cache covers n */
    if (n >= cache_len)
    {
        idx_t limit = n;
        if (cache_len == 0U)
        {
            /* initialise first term */
            ensure_cache_capacity(limit + 1U);
            cache[0] = 1;
            cache_len = 1U;
        }
        for (idx_t i = cache_len; i <= limit; ++i)
        {
            seq_t sum = 0;
            for (idx_t k = 1; ; ++k)
            {
                idx_t r = k * (2U * k - 1U);      /* exponent k*(2k-1) */
                if (r > i)
                    break;
                sum += cache[i - r];
            }
            cache[i] = -sum;
        }
        cache_len = limit + 1U;
    }
    return cache[n];
}
