/*
 * A352608  a(n) = Bell(n)*n!!.
 * https://oeis.org/A352608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000110(n)*A006882(n) (Bell number × double factorial)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached values */
static seq_t *bell   = NULL;   /* Bell numbers B(n) */
static seq_t *dblfact= NULL;   /* Double factorial n!! */
static idx_t  cache_max = (idx_t)-1;  /* highest n that has been cached */
static size_t cache_cap = 0;          /* allocated capacity of the arrays */

/* Grow and fill the cache up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Grow capacity to at least n+1 (next power of two). */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;

        /* Allocate or reallocate memory. Cast from void* to seq_t*. */
        if (!bell || !dblfact) {
            bell    = (seq_t *)calloc(new_cap, sizeof(seq_t));
            dblfact = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            bell    = (seq_t *)realloc(bell,    new_cap * sizeof(seq_t));
            dblfact = (seq_t *)realloc(dblfact, new_cap * sizeof(seq_t));
            /* Zero any newly allocated slots. */
            size_t i;
            for (i = cache_cap; i < new_cap; ++i)
                bell[i] = (seq_t)0;
        }

        /* Set zeros for existing slots that were not initialized yet. */
        if (cache_cap < new_cap) {
            memset(bell + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
            memset(dblfact + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* Compute values from cache_max+1 up to n, inclusive. */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        /* Double factorial n!! */
        if (m <= 1)
            dblfact[m] = (seq_t)1;
        else
            dblfact[m] = dblfact[m - 2] * (seq_t)m;

        /* Bell number B(m) using the recursion
           B(m) = Σ_{k=0}^{m-1} binom(m-1,k) * B(k)     */
        if (m == 0) {
            bell[m] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;               /* binom(m-1,0) */
            for (idx_t k = 0; k <= m - 1; ++k) {
                sum += bin * bell[k];
                if (k < m - 1) {
                    /* binom(m-1, k+1) = binom(m-1, k) * (m-1-k)/(k+1) */
                    bin = (bin * (seq_t)(m - 1 - k)) / (seq_t)(k + 1);
                }
            }
            bell[m] = sum;
        }
    }

    cache_max = n;
}

/* Public routine: a(n) = Bell(n) * n!!  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return bell[n] * dblfact[n];
}
