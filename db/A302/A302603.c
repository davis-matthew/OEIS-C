/*
 * A302603  Number of total dominating sets in the wheel graph on n nodes.
 * https://oeis.org/A302603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 3*a(n-3) - 2*a(n-4) - a(n-5) +
 *            2*a(n-6), with a(1)=4, a(2)=2, a(3)=4, a(4)=11, a(5)=24, a(6)=42
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, 1-based indexing (cache[0] = a(1)) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms currently stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory – expected not to happen */

        /* first six terms */
        cache[0] = (seq_t)4;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)4;   /* a(3) */
        cache[3] = (seq_t)11;  /* a(4) */
        cache[4] = (seq_t)24;  /* a(5) */
        cache[5] = (seq_t)42;  /* a(6) */
        len = 6;
    }

    if (n <= len) {
        return cache[n - 1];
    }

    /* compute terms until we reach n */
    while (len < n) {
        /* if needed, grow cache */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* indices for the recurrence (len+1 is the new term) */
        seq_t a1 = cache[len - 1]; /* a(n-1) */
        seq_t a2 = cache[len - 2]; /* a(n-2) */
        seq_t a3 = cache[len - 3]; /* a(n-3) */
        seq_t a4 = cache[len - 4]; /* a(n-4) */
        seq_t a5 = cache[len - 5]; /* a(n-5) */
        seq_t a6 = cache[len - 6]; /* a(n-6) */

        /* compute next term using the recurrence */
        seq_t pos = 4 * a1 + 3 * a3 + 2 * a6;
        seq_t neg = 5 * a2 + 2 * a4 + a5;
        seq_t next = pos - neg;     /* pos > neg for n >= 7 */

        cache[len] = next;
        ++len;
    }

    return cache[n - 1];
}
