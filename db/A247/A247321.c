/*
 * A247321  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,0) to (n,k), where 0 <= k <= 3, consisting of
 *          segments given by the vectors (1,1), (1,2), (1,-1).
 * https://oeis.org/A247321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = T(n/4, n%4) where T is defined by T(0,0)=1, T(0,k)=0
 *            (k>0), T(m,k)=T(m-1,k-1)+T(m-1,k-2)+T(m-1,k+1) for 0≤k≤3
 *            (missing indices are treated as 0)
 * Range:     u8 n <= 41, u16 n <= 76, u32 n <= 141, u64 n <= 276,
 *            u128 n <= 544, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static size_t last_row = 0;   /* last fully computed row (0‑based) */
    static seq_t prev_row[4] = {0};

    /* initialise cache with the first row (= row 0) */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* row 0 values: 1,0,0,0 */
        cache[0] = (seq_t)1; cache[1] = (seq_t)0;
        cache[2] = (seq_t)0; cache[3] = (seq_t)0;
        cache_len = 4;
        last_row = 0;
        prev_row[0] = (seq_t)1; prev_row[1] = (seq_t)0;
        prev_row[2] = (seq_t)0; prev_row[3] = (seq_t)0;
    }

    /* compute all rows up to required_row = n/4 */
    size_t required_row = n >> 2;          /* integer division by 4 */
    while (last_row < required_row) {
        /* compute next row using recurrence */
        seq_t cur[4];
        cur[0] = prev_row[1];                /* T(r,0) = T(r-1,1) */
        cur[1] = prev_row[0] + prev_row[2];  /* T(r,1) = T(r-1,0)+T(r-1,2) */
        cur[2] = prev_row[1] + prev_row[0] + prev_row[3]; /* T(r,2) = T(r-1,1)+T(r-1,0)+T(r-1,3) */
        cur[3] = prev_row[2] + prev_row[1];  /* T(r,3) = T(r-1,2)+T(r-1,1) */

        /* ensure capacity for 4 more entries */
        if (cache_len + 4 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 32;
            while (new_cap < cache_len + 4) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* allocation failure – exit silently (undefined behaviour) */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* append the new row to cache */
        cache[cache_len++] = cur[0];
        cache[cache_len++] = cur[1];
        cache[cache_len++] = cur[2];
        cache[cache_len++] = cur[3];

        /* prepare for next iteration */
        prev_row[0] = cur[0]; prev_row[1] = cur[1];
        prev_row[2] = cur[2]; prev_row[3] = cur[3];
        ++last_row;
    }

    /* index into the cache: (row * 4) + offset */
    size_t idx = (required_row << 2) + (n & 3);
    return cache[idx];
}
