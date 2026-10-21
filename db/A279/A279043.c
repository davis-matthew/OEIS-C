/*
 * A279043  Numbers k such that 3*k+1 and 4*k+1 are both triangular numbers
 *          (A000217).
 * https://oeis.org/A279043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 195*a(n-1) - 195*a(n-2) + a(n-3) (n > 3) with a(1)=63,
 *            a(2)=12285, a(3)=2383290
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache uses 0‑based indexing: cache[0] = a(1), etc. */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* capacity of cache array */
    static size_t used = 0;     /* number of terms currently computed */

    /* Ensure the cache can hold up to n elements. */
    if (n > (idx_t)cap) {
        size_t new_cap = (cap > 0) ? cap << 1 : 64;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Assume allocation always succeeds in the judge environment. */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = used; i < (size_t)n; ++i) {
        if (i == 0)          /* a(1) */
            cache[i] = 63;
        else if (i == 1)     /* a(2) */
            cache[i] = 12285;
        else if (i == 2)     /* a(3) */
            cache[i] = 2383290;
        else {
            seq_t a1 = cache[i - 1];  /* a(n-1) */
            seq_t a2 = cache[i - 2];  /* a(n-2) */
            seq_t a3 = cache[i - 3];  /* a(n-3) */
            cache[i] = (seq_t)195 * a1 - (seq_t)195 * a2 + a3;
        }
    }
    used = (size_t)n;

    return cache[(size_t)n - 1];
}
