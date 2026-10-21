/*
 * A265990  Number of nX7 integer arrays with each element equal to the number
 *          of horizontal and antidiagonal neighbors not equal to itself.
 * https://oeis.org/A265990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +16*a(n-2) -15*a(n-3) -97*a(n-4) +84*a(n-5)
 *            +268*a(n-6) -208*a(n-7) -304*a(n-8) +192*a(n-9) +64*a(n-10), for
 *            n > 15. Initial values: a(1) = 1; a(2) = 7; a(3) = 9; a(4) = 24;
 *            a(5) = 59; a(6) = 137; a(7) = 422; a(8) = 865; a(9) = 2777;
 *            a(10) = 5666; a(11) = 16753; a(12) = 35213; a(13)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* The types seq_t and idx_t are provided by the build. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 0;

/* Initial values for n = 1 .. 15 */
static const seq_t init_vals[15] = {
    1ULL,      7ULL,      9ULL,      24ULL,     59ULL,
    137ULL,    422ULL,    865ULL,    2777ULL,   5666ULL,
    16753ULL,  35213ULL,  94722ULL,  205921ULL, 510225ULL
};

static void ensure_capacity(idx_t n)
{
    /* We need to store values up to n, plus a few for recurrence safety. */
    size_t needed = (size_t)(n + 11);
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap < 64 ? 64 : cache_cap;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for offset 1, but safe fallback */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init_vals[i];
        max_computed = 15;
    }

    /* If n is already computed, return directly */
    if (n <= max_computed)
        return cache[(size_t)(n - 1)];

    /* Ensure enough capacity */
    ensure_capacity(n);

    /* Compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Indices into cache: a(k) is at cache[k-1] */
        size_t idx_nm1  = (size_t)(i - 2);  /* a(i-1)  */
        size_t idx_nm2  = (size_t)(i - 3);  /* a(i-2)  */
        size_t idx_nm3  = (size_t)(i - 4);  /* a(i-3)  */
        size_t idx_nm4  = (size_t)(i - 5);  /* a(i-4)  */
        size_t idx_nm5  = (size_t)(i - 6);  /* a(i-5)  */
        size_t idx_nm6  = (size_t)(i - 7);  /* a(i-6)  */
        size_t idx_nm7  = (size_t)(i - 8);  /* a(i-7)  */
        size_t idx_nm8  = (size_t)(i - 9);  /* a(i-8)  */
        size_t idx_nm9  = (size_t)(i -10);  /* a(i-9)  */
        size_t idx_nm10 = (size_t)(i -11);  /* a(i-10) */

        /* Positive contributions */
        seq_t pos = cache[idx_nm1];
        pos += 16 * cache[idx_nm2];
        pos += 84 * cache[idx_nm5];
        pos += 268 * cache[idx_nm6];
        pos += 192 * cache[idx_nm9];
        pos += 64 * cache[idx_nm10];

        /* Negative contributions */
        seq_t neg = 15 * cache[idx_nm3];
        neg += 97 * cache[idx_nm4];
        neg += 208 * cache[idx_nm7];
        neg += 304 * cache[idx_nm8];

        /* Final value */
        cache[(size_t)(i - 1)] = pos - neg;
    }

    max_computed = n;
    return cache[(size_t)(n - 1)];
}
