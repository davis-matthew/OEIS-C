/*
 * A375683  Expansion of e.g.f. 1 / (1 + x * (exp(x) - 1)).
 * https://oeis.org/A375683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375683
   recurrence:  a(0)=1
   a(n) = -n * Σ_{k=2..n} C(n-1,k-1) * a(n-k),   n≥1   (OEIS formula) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;   /* array holding computed a(i) */
static size_t cache_cap  = 0;      /* current allocated size (number of slots) */
static size_t cache_max  = (size_t)-1;  /* largest index that has been computed */

/* Ensure the cache can hold indices up to 'needed'.  If the cache is being
   created for the first time, initialise a(0)=1 (and a(1)=0).  The
   cache array is grown exponentially to keep realloc cost low. */
static void ensure_cache(size_t needed)
{
    if (cache != NULL && needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;

    /* First allocation – initialise the known values. */
    if (cache_max == (size_t)-1) {
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        if (cache_cap > 1)
            cache[1] = (seq_t)0;      /* a(1) will be filled by the loop,
                                         but set to 0 for safety. */
        cache_max = 0;
    }
}

seq_t a(idx_t n)
{
    size_t up = (size_t)n;          /* work with size_t for loop bounds */

    /* a(0) is always available after the first call to ensure_cache. */
    if (up == 0) {
        ensure_cache(0);
        return cache[0];
    }

    ensure_cache(up);

    /* If already computed, just return it. */
    if (up <= cache_max)
        return cache[up];

    /* Compute all missing values up to 'up' inclusively. */
    for (size_t i = cache_max + 1; i <= up; ++i) {
        seq_t sum = (seq_t)0;

        /* Sum_{k=2..i} binomial(i-1, k-1) * a(i-k) = sum_{j=1..i-1} binomial(i-1, j) * a(i-j-1) */
        seq_t bin = (seq_t)(i - 1);   /* bin = binomial(i-1, 1)  */
        for (size_t j = 1; j < i; ++j) {
            if (j > 1) {
                /* Update bin to binomial(i-1, j) */
                bin = (bin * (seq_t)(i - j)) / (seq_t)j;
            }
            sum += bin * cache[i - j - 1];
        }
        cache[i] = -((seq_t)i * sum);
    }

    cache_max = up;
    return cache[up];
}
