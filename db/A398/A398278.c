/*
 * A398278  Number of (non-null) induced trees in the grid graph P_3 X P_n.
 * https://oeis.org/A398278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence a(n) = 7*a(n-1) - 15*a(n-2) +
 *            6*a(n-3) + 31*a(n-4) - 73*a(n-5) + 75*a(n-6) - 36*a(n-7) +
 *            6*a(n-8), with initial values a(1)=6, a(2)=33, a(3)=138,
 *            a(4)=533, a(5)=2003, a(6)=7453, a(7)=27643, a(8)=102432
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[8] = {
    /* a(1) .. a(8) */
    6, 33, 138, 533, 2003, 7453, 27643, 102432
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;
    if (cache == NULL) {
        /* initialise cache with first 8 terms */
        cache_cap = 64;
        if (n > cache_cap)
            cache_cap = (size_t)n;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
        cache_len = 8;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n > new_cap)
            new_cap = (size_t)n;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    /* fill missing terms up to n */
    while (cache_len < n) {
        /* compute next term using recurrence with only positive subtractions */
        seq_t pos = 0;
        /* 7*a(n-1) */
        pos += 7 * cache[cache_len - 1];
        /* 6*a(n-3) */
        pos += 6 * cache[cache_len - 3];
        /* 31*a(n-4) */
        pos += 31 * cache[cache_len - 4];
        /* 75*a(n-6) */
        pos += 75 * cache[cache_len - 6];
        /* 6*a(n-8) */
        pos += 6 * cache[cache_len - 8];

        seq_t neg = 0;
        /* 15*a(n-2) */
        neg += 15 * cache[cache_len - 2];
        /* 73*a(n-5) */
        neg += 73 * cache[cache_len - 5];
        /* 36*a(n-7) */
        neg += 36 * cache[cache_len - 7];

        seq_t next = pos - neg;   /* pos >= neg for this sequence */
        cache[cache_len++] = next;
    }
}

seq_t a(idx_t n)
{
    if (n <= 8)
        return init_vals[n - 1];
    ensure_cache(n);
    return cache[n - 1];
}
