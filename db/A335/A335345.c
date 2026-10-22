/*
 * A335345  Expansion of e.g.f. exp(x^2/(2*(1 - x)^3)).
 * https://oeis.org/A335345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*(n-1)*a(n-1) - 2*(n-1)*(6*n-13)*a(n-2) * +
 *            (n-1)*(n-2)*(8*n-23)*a(n-3) * -
 *            2*(n-1)*(n-2)*(n-3)*(n-4)*a(n-4)) / 2 * with a(0)=1, a(1)=0,
 *            a(2)=1, a(3)=9 * (recurrence from the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External integer types provided by the build */
extern seq_t a(idx_t n);

/* ---------- internal cache handling ---------- */

static seq_t *cache = NULL;      /* array holding a[0..max_cached] */
static size_t cache_cap = 0;     /* allocated capacity of ^ */
static size_t max_cached = 0;    /* highest index computed so far */

/* Grow the cache until it contains at least `need` elements (0‑based) */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failure – abort */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for all indices we will access */
    /* `size_t` is large enough for all n we handle (here up to 10000 etc.) */
    if (n >= (idx_t)cache_cap)
        grow_cache((size_t)n + 1);

    /* For very small n we can return directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;

    /* Initialize cache for the first four values if this is the first call */
    if (max_cached < 3)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        max_cached = 3;
    }

    /* Compute any missing values up to n */
    for (size_t i = max_cached + 1; i <= (size_t)n; ++i)
    {
        /* Compute the numerator of the recurrence */
        seq_t term1 = (seq_t)8 * (seq_t)(i - 1) * cache[i - 1];
        seq_t term2 = (seq_t)2 * (seq_t)(i - 1) *
                      (seq_t)(6 * i - 13) * cache[i - 2];
        seq_t term3 = (seq_t)(i - 1) * (seq_t)(i - 2) *
                      (seq_t)(8 * i - 23) * cache[i - 3];
        seq_t term4 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)(i - 2) *
                      (seq_t)(i - 3) * (seq_t)(i - 4) * cache[i - 4];

        /* L = 8*(i-1)*a(i-1) - 2*(i-1)*(6*i-13)*a(i-2)
         *     + (i-1)*(i-2)*(8*i-23)*a(i-3)
         *     - 2*(i-1)*(i-2)*(i-3)*(i-4)*a(i-4) */
        seq_t L = term1 - term2 + term3 - term4;

        /* a(i) = L / 2 (the division is exact) */
        cache[i] = L / 2;
    }

    max_cached = (size_t)n;
    return cache[(size_t)n];
}
