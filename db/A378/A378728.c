/*
 * A378728  The total number of fires in a rooted undirected infinite 5-ary
 *          tree with a self-loop at the root, when the chip-firing process
 *          starts with (5^n-1)/4 chips at the root.
 * https://oeis.org/A378728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 12*a(n-1) - 46*a(n-2) + 60*a(n-3) - 25*a(n-4)  
   (a(1)=0, a(2)=1, a(3)=12, a(4)=98) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n <= 4) {
        switch (n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)12;
            case 4: return (seq_t)98;
        }
    }

    /* Static cache to store values up to the largest n queried so far */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 5);  /* indices 0..4 */
        if (!cache) return 0;                        /* allocation failed */
        cache[0] = 0;                                /* unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)98;
        cache_len = 4;
    }

    /* Extend cache if a larger n is requested */
    if (n > cache_len) {
        size_t new_size = (size_t)n + 1;      /* need indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) return 0;                /* allocation failed */
        cache = new_cache;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t t1 = (seq_t)12 * cache[i - 1];
            seq_t t2 = (seq_t)46 * cache[i - 2];
            seq_t t3 = (seq_t)60 * cache[i - 3];
            seq_t t4 = (seq_t)25 * cache[i - 4];
            cache[i] = t1 - t2 + t3 - t4;
        }
        cache_len = n;
    }

    return cache[n];
}
