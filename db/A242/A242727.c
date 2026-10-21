/*
 * A242727  Sum of the third largest parts of the partitions of 4n into 4
 *          parts.
 * https://oeis.org/A242727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 a(n-1) - 3 a(n-2) + 3 a(n-3) - 6 a(n-4) + 6 a(n-5) - 3
 *            a(n-6) + 3 a(n-7) - 3 a(n-8) + a(n-9) (for n ≥ 10) Base values
 *            (for n = 1 … 9): a(1)=1, a(2)=7, a(3)=29, a(4)=86, a(5)=198,
 *            a(6)=396, a(7)=719, a(8)=1203, a(9)=1899
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values */
    static const seq_t base[10] = {
        0,  /* index 0 – unused */
        1,  /* a(1) */
        7,  /* a(2) */
        29, /* a(3) */
        86, /* a(4) */
        198,/* a(5) */
        396,/* a(6) */
        719,/* a(7) */
        1203,/* a(8) */
        1899 /* a(9) */
    };

    if (n <= 9)
        return base[n];

    /* Cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* last index that has a valid entry */

    if (cache == NULL) {
        /* Allocate space for indices 0 … 9 and copy the base values */
        cache = (seq_t *)malloc(sizeof(seq_t) * 10);
        if (!cache) exit(1);  /* out‑of‑memory; terminate the program */
        for (idx_t i = 0; i <= 9; ++i) cache[i] = base[i];
        cache_max = 9;
    }

    if (n > cache_max) {
        /* Grow the cache to accommodate index n */
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
        if (!new_cache) exit(1);
        cache = new_cache;

        /* Compute new entries using the recurrence */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t pos = 0;
            pos += ((seq_t)3) * cache[i - 1];
            pos += ((seq_t)3) * cache[i - 3];
            pos += ((seq_t)6) * cache[i - 5];
            pos += ((seq_t)3) * cache[i - 7];
            pos += cache[i - 9];

            seq_t neg = 0;
            neg += ((seq_t)3) * cache[i - 2];
            neg += ((seq_t)6) * cache[i - 4];
            neg += ((seq_t)3) * cache[i - 6];
            neg += ((seq_t)3) * cache[i - 8];

            cache[i] = pos - neg;  /* guaranteed non‑negative */
        }
        cache_max = n;
    }

    return cache[n];
}
