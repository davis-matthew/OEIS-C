/*
 * A279705  Number of n X 4 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors and with new
 *          values introduced in order 0 sequentially upwards.
 * https://oeis.org/A279705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279705
 * Formula: a(n) = 10*a(n-1) - 33*a(n-2) + 61*a(n-3) - 78*a(n-4) + 64*a(n-5)
 *          - 32*a(n-6) + 9*a(n-7) - a(n-8)   for n > 9
 * Initial values (offset = 1):
 *   a(1)=2,  a(2)=4,  a(3)=22,  a(4)=125,  a(5)=717,
 *   a(6)=4121, a(7)=23690, a(8)=136181, a(9)=782826
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indexes below the offset
     * (the sequence is defined only for n ≥ 1). */
    if (n < 1) return (seq_t)0;

    /* static cache for memoised values   */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;          /* number of terms currently stored (1‑based) */

    /* initialise cache with the first 9 terms */
    if (!cache) {
        cap = 64;                     /* start with some room */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* safety, though unlikely */

        /*  index 0 is unused – keep it 0 for simplicity */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)22;
        cache[4] = (seq_t)125;
        cache[5] = (seq_t)717;
        cache[6] = (seq_t)4121;
        cache[7] = (seq_t)23690;
        cache[8] = (seq_t)136181;
        cache[9] = (seq_t)782826;
        len = 9;
    }

    /* grow cache if necessary */
    if (n > len) {
        /* extend capacity until we can hold index n */
        while (cap <= n) {
            size_t new_cap = cap * 2;
            if (new_cap == cap) new_cap++; /* avoid zero-loop on cap==0 */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return cache[n];     /* if realloc fails, return what we have */
            cache = tmp;
            cap = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t i = len + 1; i <= n; ++i) {
            seq_t val =  (seq_t)10 * cache[i-1]
                       - (seq_t)33 * cache[i-2]
                       + (seq_t)61 * cache[i-3]
                       - (seq_t)78 * cache[i-4]
                       + (seq_t)64 * cache[i-5]
                       - (seq_t)32 * cache[i-6]
                       + (seq_t)9  * cache[i-7]
                       -          cache[i-8];
            cache[i] = val;
        }
        len = n;
    }

    return cache[n];
}
