/*
 * A316213  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          4, 5 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=14, a(3)=28, a(4)=75, a(5)=250, a(6)=706,
 *            a(7)=2116, a(8)=6399, a(9)=19179, a(10)=57552, a(11)=173123,
 *            a(12)=520515, a(13)=1564516, a(14)=4704272, a(15)=14144805,
 *            a(16)=42529166,
 *            a(n)=2*a(n-1)+4*a(n-2)+5*a(n-3)-13*a(n-4)-31*a(n-5)
 *            -25*a(n-6)+30*a(n-7)+75*a(n-8)+66*a(n-9)-11*a(n-10) -...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base_terms[16] = {
    4ull, 14ull, 28ull, 75ull, 250ull, 706ull,
    2116ull, 6399ull, 19179ull, 57552ull, 173123ull,
    520515ull, 1564516ull, 4704272ull, 14144805ull, 42529166ull
};

static const int coeff[15] = {
    2, 4, 5, -13, -31, -25, 30, 75, 66, -11, -52, -18, -12, -24, -27
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of stored terms (≥ cache_len) */
    static size_t cache_cap = 0;  /* allocated capacity */

    if (n == 0) return (seq_t)0; /* out of range, undefined */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;                 /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure */
        for (size_t i = 0; i < 16; ++i)
            cache[i] = base_terms[i];
        cache_len = 16;
    }

    /* grab from cache if already computed */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* grow cache if needed */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t val = 0;
        /* for the first 16 terms, use the base table directly */
        if (i < 16) {
            cache[i] = base_terms[i];
            continue;
        }
        /* otherwise use the recurrence of order 15 */
        for (int j = 0; j < 15; ++j) {
            size_t idx = i - 1 - j;          /* term a(i-j) with 0‑based index */
            seq_t term = cache[idx];
            if (coeff[j] > 0)
                val += (seq_t)coeff[j] * term;
            else
                val -= (seq_t)(-coeff[j]) * term;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[n - 1];
}
