/*
 * A286311  a(n) = 2*a(n-1) - a(n-2) + a(n-4), n>3, a(0)=0, a(1)=a(2)=1,
 *          a(3)=3.
 * https://oeis.org/A286311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4); Initial values: a(0)=0,
 *            a(1)=a(2)=1, a(3)=3
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cap   = 0;      /* allocated capacity (in terms of seq_t items) */
    static idx_t  used  = 0;     /* number of initialized items (up to used-1) */

    /* initialise cache on first call – store a(0)..a(3) */
    if (!cache) {
        cap  = 4;
        cache = (seq_t *)malloc(cap * sizeof *cache);
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        used = 4;
    }

    /* already computed? */
    if (n < used) return cache[n];

    /* grow cache to reach index n */
    for (idx_t i = used; i <= n; ++i) {
        /* ensure capacity for index i */
        while ((size_t)i >= cap) {
            size_t new_cap = cap ? cap * 2 : 4;
            if (new_cap < (size_t)i + 1) new_cap = (size_t)i + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof *cache);
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* recurrence: a(i) = 2*a(i-1) - a(i-2) + a(i-4) */
        seq_t val = cache[i - 1];
        val += val;            /* 2*a(i-1) */
        val -= cache[i - 2];    /* - a(i-2) */
        val += cache[i - 4];    /* + a(i-4) */
        cache[i] = val;
    }

    used = n + 1;
    return cache[n];
}
