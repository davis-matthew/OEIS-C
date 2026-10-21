/*
 * A281468  Number of nX7 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A281468
 * Empirical recurrence (valid for n > 19):
 *   a(n) = 2*a(n-1) + 3*a(n-3) - 4*a(n-4) - 4*a(n-5) + a(n-7)
 *          + 2*a(n-8) - a(n-9) + 5*a(n-10) - 3*a(n-12)
 * Initial values (n = 1 … 24):
 *   64, 361, 145, 260, 472, 924, 1887, 3681, 7126, 14363,
 *   29371, 59830, 122230, 250638, 513616, 1053242, 2163243,
 *   4442634, 9122210, 18740008, 38502723, 79097291,
 *   162508103, 333905314
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static data ---------- */
static const seq_t init_values[24] = {
    64ULL, 361ULL, 145ULL, 260ULL, 472ULL, 924ULL, 1887ULL, 3681ULL,
    7126ULL, 14363ULL, 29371ULL, 59830ULL, 122230ULL, 250638ULL,
    513616ULL, 1053242ULL, 2163243ULL, 4442634ULL, 9122210ULL,
    18740008ULL, 38502723ULL, 79097291ULL, 162508103ULL, 333905314ULL
};

/* ---------- cache (dynamic, 1‑based index) ---------- */
static seq_t *cache = NULL;       /* points to an array of seq_t */
static size_t cache_size = 0;     /* allocated capacity (elements) */
static idx_t  cache_max = 0;      /* largest index computed (1‑based) */

/* ---------- helper functions ---------- */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_size)
        return;
    size_t new_cap = cache_size ? cache_size * 2 : 24;
    while (new_cap < (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_size = new_cap;
}

static void init_cache(void)
{
    if (cache)
        return;
    cache_size = 24;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    for (size_t i = 0; i < 24; ++i)
        cache[i] = init_values[i];
    cache_max = 24;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n == 0 */

    init_cache();

    if (n <= cache_max)
        return cache[n - 1];

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* no need to grow for k <= 24 because init_cache handled them */
        if (k <= 24)
            continue;

        ensure_capacity(k);

        seq_t pos = 0, neg = 0;

        /* positive terms */
        pos += (seq_t)2 * cache[k - 2];     /* 2*a(k-1)   */
        pos += (seq_t)3 * cache[k - 4];     /* 3*a(k-3)   */
        pos +=                cache[k - 8]; /*   a(k-7)   */
        pos += (seq_t)2 * cache[k - 9];     /* 2*a(k-8)   */
        pos += (seq_t)5 * cache[k -11];     /* 5*a(k-10)  */

        /* negative terms */
        neg += (seq_t)4 * cache[k - 5];     /* 4*a(k-4)   */
        neg += (seq_t)4 * cache[k - 6];     /* 4*a(k-5)   */
        neg +=                cache[k -10]; /*   a(k-9)   */
        neg += (seq_t)3 * cache[k -13];     /* 3*a(k-12)  */

        cache[k - 1] = pos - neg;
    }

    cache_max = n;
    return cache[n - 1];
}
