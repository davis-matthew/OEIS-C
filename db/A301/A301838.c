/*
 * A301838  Number of nX5 0..1 arrays with every element equal to 0, 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301838
   Recurrence (for n > 13):
      a(n) = 24*a(n-1) - 201*a(n-2) + 885*a(n-3) - 2461*a(n-4) + 4680*a(n-5)
            - 6187*a(n-6) + 5625*a(n-7) - 3450*a(n-8) + 1378*a(n-9)
            - 324*a(n-10) + 32*a(n-11)
   Initial values (1‑indexed):
      a(1)=16,   a(2)=263,   a(3)=3182,   a(4)=39703,   a(5)=496085,
      a(6)=6196305, a(7)=77396422, a(8)=966770632, a(9)=12076215811,
      a(10)=150848052398, a(11)=1884295439869, a(12)=23537397007865,
      a(13)=294013935342365 */
#include <stdlib.h>   /* malloc, realloc, free, size_t */
#include <stddef.h>   /* NULL */
#include <stdbool.h>  /* bool */

/* static cache for computed values, 1‑indexed */
static seq_t *cache = NULL;    /* array holding values a(0)…a(n) */
static size_t cache_cap = 0;   /* currently allocated capacity (>= desired size) */

/* ensure the cache can hold indices up to ‘need’ (inclusive) */
static void
ensure_cache(idx_t need)
{
    /* needed size includes index 0 (unused) */
    size_t needed = (size_t)need + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    /* realloc and cast to seq_t* (works in both C and C++) */
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        return;  /* allocation failed – caller will get 0, acceptable for this task */

    /* initialize newly allocated slots to zero (seq_t zeros may not be all bits zero) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        p[i] = (seq_t)0;

    cache = p;
    cache_cap = new_cap;
}

/* base values 1…13 (index 0 unused) */
static const seq_t base_vals[14] = {
    (seq_t)0,                 /* unused a(0) */
    (seq_t)16,
    (seq_t)263,
    (seq_t)3182,
    (seq_t)39703,
    (seq_t)496085,
    (seq_t)6196305,
    (seq_t)77396422,
    (seq_t)966770632,
    (seq_t)12076215811,
    (seq_t)150848052398,
    (seq_t)1884295439869,
    (seq_t)23537397007865,
    (seq_t)294013935342365
};

/* recurrence coefficients (a(n-k-1) multiplied by coeffs[k]) */
static const int coeffs[11] = {
    24,  -201, 885, -2461, 4680,
   -6187, 5625, -3450, 1378, -324, 32
};

/* ------------------------------------------------------------------ */
/* compute a(n)  for n >= 1, any order of calls, memoised */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* return stored base values directly, no need to touch cache */
    if (n <= 13)
        return base_vals[(size_t)n];

    /* make sure cache can hold indices up to n */
    ensure_cache(n);

    /* initialise base values in cache if not already present */
    for (size_t i = 1; i <= 13; ++i)
        if (cache[i] == (seq_t)0)
            cache[i] = base_vals[i];

    /* compute values up to n (skipping already known ones) */
    for (idx_t i = (idx_t)14; i <= n; ++i)
    {
        if (cache[i] != (seq_t)0)   /* already computed by earlier call */
            continue;

        seq_t sum = (seq_t)0;
        for (int k = 0; k < 11; ++k)
        {
            /* multiply previous term by the coefficient, allow negative coeffs */
            sum += cache[i - (idx_t)(k + 1)] * (seq_t)coeffs[k];
        }
        cache[i] = sum;
    }

    return cache[n];
}
