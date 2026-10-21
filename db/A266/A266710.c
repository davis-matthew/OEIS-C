/*
 * A266710  Coefficient of x^0 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5) +
 *            p(n) where p(n) = -1, 6, 1, 46, 301, -260, -92, 18 for n=0..7,
 *            and p(n) = 0 for n >= 8
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266710 - coefficient of x^0 in the minimal polynomial of
   [1^n, sqrt(2), 1,1,...) */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;       /* allocated capacity */
static size_t max_comp = 0;        /* highest index computed, inclusive */

/* p(n) values for n = 0..7 */
static const seq_t P[8] = {
    -1,      /* p(0) */
     6,      /* p(1) */
     1,      /* p(2) */
    46,      /* p(3) */
   301,      /* p(4) */
  -260,      /* p(5) */
  -92,       /* p(6) */
   18        /* p(7) */
};

/* Ensure cache is large enough for indices 0..n and compute missing values. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n > (idx_t)max_comp) {
        size_t start = max_comp + 1;
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t term = 5 * cache[i-1]
                       + 15 * cache[i-2]
                       - 15 * cache[i-3]
                       - 5  * cache[i-4]
                       +    cache[i-5];
            if (i <= 7) term += P[i];
            cache[i] = term;
        }
        max_comp = (size_t)n;
    }
}

seq_t a(idx_t n)
{
    /* Lazy initialization of cache with initial terms */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure, unlikely in tests */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t) 1;
        cache[2] = (seq_t)-9;
        cache[3] = (seq_t)31;
        cache[4] = (seq_t)311;
        max_comp = 4;
    }

    ensure_cache(n);
    return cache[(size_t)n];
}
