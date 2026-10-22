/*
 * A318610  a(1) = 0, a(2) = 4, a(3) = 12; for n > 3, a(n) = 3*a(n-1) -
 *          3*a(n-2) + 9*a(n-3).
 * https://oeis.org/A318610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=4, a(3)=12; a(n)=3*a(n-1)-3*a(n-2)+9*a(n-3) for n>3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 3) {
        if (n == 1) return (seq_t)0;
        if (n == 2) return (seq_t)4;
        return (seq_t)12;          /* n == 3 */
    }

    /* static cache to allow random order calls and reuse */
    static seq_t *cache = NULL;   /* index 1.. */
    static idx_t cached_n = 0;    /* largest n computed so far */
    static size_t cap = 0;        /* allocated size of cache (bytes) */

    /* grow allocation if needed */
    if (n >= cap) {
        size_t new_cap = (size_t)(n + 1);        /* one entry per index, plus index 0 dummy */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();               /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* initialize first terms if cache was just created */
    if (cached_n == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cached_n = 3;
    }

    /* extend cache up to requested n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-2) + 9*a(i-3) */
        cache[i] = (seq_t)3 * cache[i-1] - (seq_t)3 * cache[i-2] + (seq_t)9 * cache[i-3];
    }
    cached_n = n;

    return cache[n];
}
