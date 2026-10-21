/*
 * A292077  a(n) = 0 if n=1; a(n) = 1-a(n-2) if n is odd; a(n) = 1-a(n/2) if n
 *          is even.
 * https://oeis.org/A292077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 0;
 *          if n is odd: a(n) = 1 - a(n-2);
 *          if n is even: a(n) = 1 - a(n/2);
 * Recurrence as described in the OEIS entry.
 */
#include <stdlib.h>

/* Return value of OEIS sequence A292077 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* holds values a(1), a(2), … */
    static size_t cap   = 0;        /* allocated capacity (number of entries) */
    static idx_t sz     = 0;        /* number of values currently stored */

    /* Expand cache if needed (using doubling strategy). */
    if ((size_t)(n + 1) > cap) {
        size_t newcap = (cap == 0 ? 16 : cap * 2);
        while (newcap <= (size_t)(n + 1))
            newcap *= 2;
        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) {
            /* Out of memory – in competitive settings this is unlikely. */
            exit(EXIT_FAILURE);
        }
        cache = newptr;
        cap = newcap;
    }

    /* Compute missing values up to n. */
    for (idx_t i = sz + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)0;
        } else if (i & 1) {                /* odd: use a(i-2) */
            cache[i] = (seq_t)1 - cache[i - 2];
        } else {                           /* even: use a(i/2) */
            cache[i] = (seq_t)1 - cache[i / 2];
        }
    }
    sz = (sz < n) ? n : sz;  /* update stored size */

    return cache[n];
}
