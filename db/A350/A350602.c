/*
 * A350602  G.f. = ((2+3*x)/(1-x^2-x^3))^2.
 * https://oeis.org/A350602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2)+2*a(n-3)-a(n-4)-2*a(n-5)-a(n-6) (n>=6) with
 *            initial terms a(0)=4, a(1)=12, a(2)=17, a(3)=32, a(4)=54,
 *            a(5)=78
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* allocated array of terms */
static idx_t cache_len = 0;       /* number of terms currently stored (max index + 1) */
static size_t cache_cap = 0;     /* current allocation size */

/* Extend the cache up to index n inclusively */
static void extend_cache(idx_t n)
{
    if (cache == NULL) {
        /* start with a modest capacity */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    /* increase capacity if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory, terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base terms if not already done */
    if (cache_len < 6) {
        static const seq_t base[] = {4, 12, 17, 32, 54, 78};
        for (idx_t i = cache_len; i < 6 && i <= n; ++i) {
            cache[i] = base[i];
        }
        if (cache_len < 6) cache_len = 6;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 6) continue; /* base terms already set */
        seq_t val = 0;
        val += 2 * cache[i - 2];
        val += 2 * cache[i - 3];
        val -= cache[i - 4];
        val -= 2 * cache[i - 5];
        val -= cache[i - 6];
        cache[i] = val;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n >= cache_len) {
        extend_cache(n);
    }
    return cache[n];
}
