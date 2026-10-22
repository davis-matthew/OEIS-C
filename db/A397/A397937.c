/*
 * A397937  a(n) = floor(7^n/4).
 * https://oeis.org/A397937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(7^n/4). Computed with recurrence a(0) = 0, a(n) =
 *            7*a(n-1) + 3 + 2*(-1)^n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence values: a(1) = 1, a(2) = 12, a(3) = 85, … */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t     cap   = 0;
    static size_t     used  = 0;  /* number of elements stored (0..used-1) */

    /* Grow cache if necessary */
    if (n + 1 > used) {
        size_t need = n + 1;
        if (cap < need) {
            size_t new_cap = (cap > 0) ? cap : 8;
            while (new_cap < need)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }

        /* Initialize from stored state up to n */
        if (used == 0) {
            cache[0] = (seq_t)0;
            used = 1;
        }

        for (idx_t i = used; i <= n; ++i) {
            seq_t add = (i & 1) ? (seq_t)1 : (seq_t)5;   /* 3 + 2*(-1)^i */
            cache[i] = 7 * cache[i - 1] + add;
        }
        used = n + 1;
    }

    return cache[n];
}
