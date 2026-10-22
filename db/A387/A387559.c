/*
 * A387559  Number of matchings in the n-Cameron graph.
 * https://oeis.org/A387559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*a(n-1) - 428*a(n-2) - 2758*a(n-3) + 2110*a(n-4) +
 *            406*a(n-5) - 288*a(n-6) + 34*a(n-7) - a(n-8) with initial
 *            values: a(1)=328, a(2)=34656, a(3)=3663196, a(4)=387220788,
 *            a(5)=40931551142, a(6)=4326710290084, a(7)=457359216406718,
 *            a(8)=48345611095683868
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for computed terms.  Index 0 is unused. */
    static seq_t *cache = NULL;    /* array of size >= n+1 */
    static idx_t cap   = 0;        /* current capacity of cache */
    static idx_t last  = 0;        /* highest index computed so far */

    /* Recurrence base values for n=1..8.  The literals are cast to seq_t
       to ensure correct type even for arbitrary‑precision builds. */
    static const seq_t base[8] = {
        (seq_t)328ULL,
        (seq_t)34656ULL,
        (seq_t)3663196ULL,
        (seq_t)387220788ULL,
        (seq_t)40931551142ULL,
        (seq_t)4326710290084ULL,
        (seq_t)457359216406718ULL,
        (seq_t)48345611095683868ULL
    };

    /* 0 is outside the offset; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* One‑time allocation for the first 8 terms.  The array is
       size 9 (indices 0..8) but 0 is unused. */
    if (cache == NULL) {
        cap   = 9;  /* capacity to hold indices 0..8 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)  /* very unlikely, but guard against OOM */
            return (seq_t)0;
        for (idx_t i = 1; i <= 8; ++i)
            cache[i] = base[i - 1];
        last = 8;
    }

    /* Grow the cache if the requested index is outside the current
       capacity.  The capacity is set to n+1 so that indices 0..n
       are valid.  realloc(0, ...) is defined and behaves like malloc,
       but here cache is non‑NULL, so we can safely call realloc. */
    if (n >= cap) {
        cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp)  /* OOM – fall back to previous cache */
            return (seq_t)0;
        cache = tmp;
    }

    /* For indices ≥ 9 compute missing values using the recurrence. */
    if (last < 8)
        for (idx_t i = last + 1; i <= 8; ++i)
            cache[i] = base[i - 1];
    if (last < 8)
        last = 8;

    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 110 * cache[i - 1];
        val -= 428 * cache[i - 2];
        val -= 2758 * cache[i - 3];
        val += 2110 * cache[i - 4];
        val += 406 * cache[i - 5];
        val -= 288 * cache[i - 6];
        val += 34 * cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
