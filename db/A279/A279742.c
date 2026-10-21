/*
 * A279742  Number of 2 X n 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) – a(n-2) – 3*a(n-3) + a(n-4) + a(n-5) Initial
 *            values: a(1) = 0 a(2) = 2 a(3) = 6 a(4) = 14 a(5) = 26
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 41, u64 n <= 86, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1 – return 0 as OEIS specifies offset 1 */
    if (n < 1)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of terms, index 0 unused */
    static size_t cache_size = 0;    /* allocated size (>= index + 1) */
    static idx_t cache_end   = 0;    /* highest index computed */

    /* ensure space for indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            abort();                 /* unrecoverable allocation failure */
        cache      = tmp;
        cache_size = needed;
    }

    /* Initialise the first 5 terms when not yet present */
    if (cache_end < 5) {
        /* we always store term 1..5 (or up to n if n < 5) */
        if (n >= 1) cache[1] = (seq_t)0;   /* a(1) */
        if (n >= 2) cache[2] = (seq_t)2;   /* a(2) */
        if (n >= 3) cache[3] = (seq_t)6;   /* a(3) */
        if (n >= 4) cache[4] = (seq_t)14;  /* a(4) */
        if (n >= 5) cache[5] = (seq_t)26;  /* a(5) */
        cache_end = (cache_end < 5) ? 5 : cache_end; /* at least 5 if n>=5 */
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_end + 1; i <= n; ++i) {
        /* recurrence a(i) = 3*a(i-1) – a(i-2) – 3*a(i-3) + a(i-4) + a(i-5) */
        seq_t term = cache[i-1] * (seq_t)3;   /* 3*a(i-1) */
        term -= cache[i-2];                  /* – a(i-2) */
        term -= cache[i-3] * (seq_t)3;       /* – 3*a(i-3) */
        term += cache[i-4];                  /* + a(i-4) */
        term += cache[i-5];                  /* + a(i-5) */
        cache[i] = term;
    }
    cache_end = n;

    return cache[n];
}
