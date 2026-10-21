/*
 * A268493  Orbit of 3 under the map A268488: n -> least number k of the form
 *          k = n*(last digit of k) + (k without its last digit).
 * https://oeis.org/A268493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10*a(n-1)-1 unless a(n-1)*10-1 is divisible by 3 or 9,
 *            then a(n) = (10*a(n-1)-1)/{3,9}, respectively
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 3.  The function generates the terms of OEIS A268493
   – the orbit of 3 under the map n -> least k of the form
   k = n*(last digit of k) + floor(k/10).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = 3, cache[1] = a(1), ...      */
    static size_t len   = 0;      /* number of elements stored in cache      */
    static size_t cap   = 0;      /* capacity of the cached array             */

    /* Initialise cache with the first value when first called. */
    if (len == 0) {
        cap   = 8;                       /* initial capacity, arbitrary */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();             /* out of memory – abort */
        cache[0] = (seq_t)3;             /* a(0) */
        len = 1;
    }

    /* Expand capacity if needed.  Use doubling strategy. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 8 : cap;
        while (new_cap < ((size_t)n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to index n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t x    = prev * (seq_t)10 - (seq_t)1;

        if ((x % (seq_t)9) == (seq_t)0) {
            cache[i] = x / (seq_t)9;
        } else if ((x % (seq_t)3) == (seq_t)0) {
            cache[i] = x / (seq_t)3;
        } else {
            cache[i] = x;
        }
    }
    len = (size_t)n + 1;   /* we now have cache[0..n] */

    return cache[(size_t)n];
}
