/*
 * A372760  2nd row of the 3-Zeckendorf array (A136189), including prepended
 *          terms.
 * https://oeis.org/A372760
 *
 * Offset:    -5    (a(-5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A372760 – 2nd row of the 3‑Zeckendorf array
 *
 *  Recurrence:  a(n) = a(n-1) + a(n-3)  (for n ≥ –2)
 *  Initial values (offset –5):
 *     a(–5) = 0,  a(–4) = 2,  a(–3) = 1,  a(–2) = 1,  a(–1) = 3
 *
 *  The function returns a(n) for every n ≥ –5; for n < –5 it returns 0.
 *  It memoises computed values in a static cache, so calls can be in any
 *  order and are efficient for n up to about 10 000.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lookup range starts at n = –5 → position 0 */
    if (n < -5)             /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    const idx_t OFFSET = 5;          /* n = –5 corresponds to pos 0 */
    idx_t pos = n + OFFSET;          /* array index for the given n */

    /* Static cache: values for indices 0 … max_pos (n+5) */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;      /* current allocated size */
    static idx_t max_pos   = -1;     /* highest index that has been computed */

    /* Initialise cache with the five known values once */
    if (cache == NULL) {
        cache_sz = 5;                /* positions 0 … 4 (n = –5 … –1) */
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;        /* allocation failure → 0 */
        cache[0] = (seq_t)0;        /* a(–5) */
        cache[1] = (seq_t)2;        /* a(–4) */
        cache[2] = (seq_t)1;        /* a(–3) */
        cache[3] = (seq_t)1;        /* a(–2) */
        cache[4] = (seq_t)3;        /* a(–1) */
        max_pos = 4;                /* last computed position */
    }

    /* Grow the cache if we need more space */
    if ((size_t)pos >= cache_sz) {
        size_t new_sz = (size_t)(pos + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;        /* allocation failure → 0 */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* Compute missing values up to the required position */
    if (pos > max_pos) {
        for (idx_t i = max_pos + 1; i <= pos; ++i) {
            if (i >= 5)          /* recurrence applies only from n = 0 (pos = 5) */
                cache[i] = cache[i - 1] + cache[i - 3];
        }
        max_pos = pos;
    }

    return cache[pos];
}
