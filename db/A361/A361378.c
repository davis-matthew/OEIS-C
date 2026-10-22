/*
 * A361378  Number of musical scales in n tone equal temperament respecting
 *          the property that alternate notes are 3 or 4 semitones apart.
 * https://oeis.org/A361378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) - a(n-6) with initial values:
 *            a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=3, a(6)=3
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static size_t len   = 0;    /* number of entries stored (0..len-1) */

    if (n < 1) /* offset starts at 1 – return 0 for out‑of‑range calls */
        return 0;

    size_t need = (size_t)n + 1;   /* we need indices 0..n */

    if (need > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();   /* fatal error */
        cache = tmp;
        cap   = newcap;
    }

    if (len == 0) {
        cache[0] = 0;  /* unused, but convenient placeholder */
        len = 1;
    }

    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 1)      cache[i] = 0;
        else if (i == 2) cache[i] = 1;
        else if (i == 3) cache[i] = 2;
        else if (i == 4) cache[i] = 3;
        else if (i == 5) cache[i] = 3;
        else if (i == 6) cache[i] = 3;
        else             cache[i] = cache[i-2] + cache[i-3] +
                                  cache[i-4] - cache[i-6];
    }

    len = need;   /* now cache[0..n] are valid */
    return cache[(size_t)n];
}
