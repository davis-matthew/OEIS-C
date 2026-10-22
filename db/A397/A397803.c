/*
 * A397803  a(n) = 2^((3^n - 1)/2).
 * https://oeis.org/A397803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^((3^n - 1)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Recurrence: a(0) = 1, a(k+1) = 2*a(k)^3 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms   */
    static size_t   cap    = 0;           /* current allocation size   */
    static idx_t   max_comp = 0;          /* highest index already filled */

    /* First time: create space for a(0) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        max_comp = 0;
    }

    /* Ensure we have room for index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cap * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Generate missing entries up to n */
    while (max_comp < n) {
        idx_t  next = max_comp + 1;
        seq_t  cur  = cache[max_comp];
        seq_t  sq   = cur * cur;   /* cur^2 */
        seq_t  cu   = sq * cur;    /* cur^3 */
        cache[next] = cu * (seq_t)2;   /* 2*cur^3 = a(next) */
        ++max_comp;
    }

    return cache[n];
}
