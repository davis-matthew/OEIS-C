/*
 * A263375  Number of (n+1) X (1+1) 0..4 arrays with each row and column
 *          divisible by 9, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+3*a(n-2)-9*a(n-3)+3*a(n-6)-9*a(n-7)-9*a(n-8)+27*a(n-9)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with seed values
   a(1)=2,a(2)=4,a(3)=11,a(4)=31,a(5)=89,a(6)=253,a(7)=748,a(8)=2214,a(9)=6615. */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

static seq_t *cache = NULL;  /* 1‑based cache, cache[0] unused */
static idx_t  cache_len = 0; /* highest n computed */
static size_t cache_cap = 0; /* allocated capacity (number of elements) */

/* Ensure cache can hold at least 'needed' elements (0 … needed‑1).  */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initialise cache with the nine seed values.  */
static void init_cache_seed(void)
{
    if (cache_len == 0) {
        ensure_cache_capacity(10);            /* need indices 0…9 */
        cache[0] = (seq_t)0;   /* unused */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)31;
        cache[5] = (seq_t)89;
        cache[6] = (seq_t)253;
        cache[7] = (seq_t)748;
        cache[8] = (seq_t)2214;
        cache[9] = (seq_t)6615;
        cache_len = 9;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    init_cache_seed();                   /* make sure seeds are present */

    /* ensure we have space for index n */
    ensure_cache_capacity(n + 1);

    if (n <= cache_len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* The recurrence is guaranteed to stay non‑negative. */
        seq_t term = (seq_t)0;
        term += (seq_t)3 * cache[i - 1];
        term += (seq_t)3 * cache[i - 2];
        term -= (seq_t)9 * cache[i - 3];
        term += (seq_t)3 * cache[i - 6];
        term -= (seq_t)9 * cache[i - 7];
        term -= (seq_t)9 * cache[i - 8];
        term += (seq_t)27 * cache[i - 9];
        cache[i] = term;
    }
    cache_len = n;
    return cache[n];
}
