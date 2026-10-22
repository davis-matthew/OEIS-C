/*
 * A384311  a(n) is the number of ways to partition an n X n X n cube into 4
 *          noncongruent cuboids.
 * https://oeis.org/A384311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A384311
   a(n) = -a(n-1) + a(n-2) + 3*a(n-3) + 3*a(n-4) - a(n-5) 
          - 4*a(n-6) - 4*a(n-7) - a(n-8) + 3*a(n-9) 
          + 3*a(n-10) + a(n-11) - a(n-12) - a(n-13), for n>13
   a(1)=0, a(2)=0, a(3)=4, a(4)=12, a(5)=47, a(6)=85, a(7)=183,
   a(8)=266, a(9)=466, a(10)=613, a(11)=941, a(12)=1179, a(13)=1668.
   The function below memoises the values and works for any n>=1. */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[13] = {
    (seq_t)0ULL, (seq_t)0ULL, (seq_t)4ULL, (seq_t)12ULL, (seq_t)47ULL,
    (seq_t)85ULL, (seq_t)183ULL, (seq_t)266ULL, (seq_t)466ULL,
    (seq_t)613ULL, (seq_t)941ULL, (seq_t)1179ULL, (seq_t)1668ULL
};

static seq_t *cache = NULL;          /* array of computed values  (0‑based) */
static size_t cache_cap = 0;         /* number of allocated slots      */
static size_t cache_len = 0;         /* number of values actually stored */

/* Ensure cache can hold at least n values (1‑based). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n;
    if (needed < 13) needed = 13;               /* store initial 13 anyway */

    if (cache_cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return;                        /* allocation failed */
        cache = tmp;
        cache_cap = needed;
        /* initialise new slots to zero */
        for (size_t i = cache_len; i < needed; ++i) cache[i] = 0;
    }

    if (cache_len < 13) {                       /* initialise first 13 terms */
        for (size_t i = 0; i < 13; ++i) cache[i] = init_vals[i];
        cache_len = 13;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for n<1 */

    ensure_cache(n);

    if (n <= 13) return init_vals[n - 1];

    /* compute missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1;                     /* zero‑based index */

        /* recurrence: a(i) = ... using cache[0..idx-1] */
        seq_t val = cache[idx - 2]          + 3 * cache[idx - 3]
                 + 3 * cache[idx - 4]          + 3 * cache[idx - 9]
                 + 3 * cache[idx -10]          +      cache[idx -11];
        val -= cache[idx - 1];
        val -= cache[idx - 5];
        val -= 4 * cache[idx - 6];
        val -= 4 * cache[idx - 7];
        val -= cache[idx - 8];
        val -= cache[idx -12];
        val -= cache[idx -13];

        cache[idx] = val;
    }
    cache_len = (size_t)n;                     /* now up to date */

    return cache[n - 1];
}
