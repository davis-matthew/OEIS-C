/*
 * A281649  Number of nX3 0..2 arrays with no element equal to more than one
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (valid for n > 13) * * a(n) = 14*a(n-1) -
 *            29*a(n-2) - 192*a(n-3) + 136*a(n-4) * + 1336*a(n-5) +
 *            1164*a(n-6) - 2528*a(n-7) * - 6176*a(n-8) - 5120*a(n-9) -
 *            1600*a(n-10) * * Initial values (a(1) … a(13)): * 1, 70, 985,
 *            10572, 104815, 982206, 8921869, * 79152992, 690169163, 59369...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free, abort */

static const seq_t init_vals[13] = {
    (seq_t)1,
    (seq_t)70,
    (seq_t)985,
    (seq_t)10572,
    (seq_t)104815,
    (seq_t)982206,
    (seq_t)8921869,
    (seq_t)79152992,
    (seq_t)690169163,
    (seq_t)5936924514ULL,
    (seq_t)50519131945ULL,
    (seq_t)426076889988ULL,
    (seq_t)3566942546599ULL
};

static seq_t *cache = NULL;      /* dynamically grown cache */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_size = 0;    /* number of initialized entries */

/* grow the cache so that it can hold at least n elements */
static void ensure_capacity(size_t n)
{
    if (n <= cache_cap)
        return;

    seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
    if (!new_cache)
        abort();  /* memory allocation failure */

    /* copy existing values */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* initialise the first 13 terms */
    for (size_t i = 0; i < 13 && i < n; ++i)
        new_cache[i] = init_vals[i];

    /* discard old cache */
    if (cache)
        free(cache);

    cache = new_cache;
    cache_cap = n;
    if (cache_size < 13)
        cache_size = 13;
}

seq_t a(idx_t n)
{
    /* return known initial terms directly */
    if (n <= 13)
        return init_vals[n - 1];

    size_t need = (size_t)n;
    ensure_capacity(need);

    /* compute missing terms using the recurrence */
    for (size_t k = cache_size + 1; k <= need; ++k) {
        /* indices are 1‑based; cache is 0‑based */
        const size_t i1  = k - 2;   /* a(k-1) */
        const size_t i2  = k - 3;   /* a(k-2) */
        const size_t i3  = k - 4;   /* a(k-3) */
        const size_t i4  = k - 5;   /* a(k-4) */
        const size_t i5  = k - 6;   /* a(k-5) */
        const size_t i6  = k - 7;   /* a(k-6) */
        const size_t i7  = k - 8;   /* a(k-7) */
        const size_t i8  = k - 9;   /* a(k-8) */
        const size_t i9  = k -10;   /* a(k-9) */
        const size_t i10 = k -11;   /* a(k-10) */

        seq_t pos = (seq_t)14  * cache[i1] + (seq_t)136  * cache[i4] +
                    (seq_t)1336 * cache[i5] + (seq_t)1164 * cache[i6];
        seq_t neg = (seq_t)29   * cache[i2] + (seq_t)192  * cache[i3] +
                    (seq_t)2528 * cache[i7] + (seq_t)6176 * cache[i8] +
                    (seq_t)5120 * cache[i9] + (seq_t)1600 * cache[i10];

        cache[k - 1] = pos - neg;   /* guaranteed non‑negative */
    }

    cache_size = need;
    return cache[n - 1];
}
