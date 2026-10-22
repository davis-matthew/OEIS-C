/*
 * A393911  a(n) = 1 + n*(n+1)/2 + Sum_{k=0..n-1} a(k) * a(n-1-k).
 * https://oeis.org/A393911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A393911:
   a(0) = 1
   a(n) = 1 + n*(n+1)/2 + Σ_{k=0}^{n-1} a(k) * a(n-1-k)
   (OEIS offset 0)
*/

#include <stdlib.h>

/* --- Memoised sequential recurrence implementation --- */

static seq_t *cache = NULL;     /* cached values a[0..len-1]          */
static size_t cache_cap = 0;    /* size of allocated memory (power of 2) */
static size_t cache_len = 0;    /* number of computed entries (1 for a[0]) */

/* Ensure a[0..n] are computed and stored in cache */
static void ensure_cache(idx_t n)
{
    /* nothing to do if already computed */
    if (cache_len > (size_t)n)
        return;

    size_t need = (size_t)n + 1;              /* at least this many entries */
    if (cache_cap < need) {                   /* enlarge allocation if required */
        size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing terms sequentially */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {                         /* a(0) = 1 */
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        for (size_t k = 0; k <= i - 1; ++k) { /* convolution part */
            sum += cache[k] * cache[i - 1 - k];
        }

        seq_t tri = ((seq_t)i * ((seq_t)i + 1)) / 2;
        cache[i] = 1 + tri + sum;
    }

    cache_len = (size_t)n + 1;
}

/* Public interface */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
