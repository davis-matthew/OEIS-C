/*
 * A317804  Numbers of form 2^i*12^j, with i, j >= 0.
 * https://oeis.org/A317804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^i * 12^j for i, j ≥ 0 (values listed in increasing
 *            order)
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 163, u64 n <= 612,
 *            u128 n <= 2367, big any n
 * Validated: standard, pass, 63 terms through n = 9633 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current capacity of the cache */
    static size_t len   = 0;   /* number of terms currently stored */
    static size_t p2    = 0;   /* pointer for generator 2   */
    static size_t p12   = 0;   /* pointer for generator 12  */

    if (n == 0)            /* undefined offset – return 0 for safety */
        return 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;            /* allocation failed */
        cache[0] = 1;                    /* a(1) = 1   */
        len = 1;
        p2 = p12 = 0;
    }

    /* Extend the cache until we have at least n terms */
    while (len < n) {
        seq_t cand2  = cache[p2]  * (seq_t)2;
        seq_t cand12 = cache[p12] * (seq_t)12;
        seq_t next   = (cand2 <= cand12) ? cand2 : cand12;

        /* Resize cache if needed */
        if (len >= cap) {
            cap = cap ? (cap << 1) : 16;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (!tmp) return 0;                    /* reallocation failed */
            cache = tmp;
        }

        cache[len++] = next;

        if (next == cand2) p2++;
        if (next == cand12) p12++;
    }

    return cache[n - 1];
}
