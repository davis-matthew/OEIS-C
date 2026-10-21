/*
 * A261306  a(n+1) = abs((n) - gcd(a(n), 6*n+5)), a(1) = 1.
 * https://oeis.org/A261306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = abs(a(n) - gcd(a(n), 6*n+5)); a(1) = 1
 * Range:     u8 n <= 46, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       (offset 1 is implied but callers will always use n >= 1). */
    if (n == 0) return 0;

    /* Static cache for memoisation.  cache[0] holds a(1). */
    static seq_t *cache   = NULL;
    static idx_t   size   = 0;        /* number of terms currently stored */
    static size_t  capacity = 0;      /* allocated capacity of cache */

    /* Ensure cache can hold terms up to index n (1‑based). */
    if (n > size) {
        size_t needed = (size_t)n;
        if (needed > capacity) {
            size_t new_cap = capacity ? capacity : 1;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;     /* allocation failed */
            cache = tmp;
            capacity = new_cap;
        }

        /* Initialise first term if the cache is empty. */
        if (size == 0) {
            cache[0] = 1;          /* a(1) = 1 */
            size = 1;
        }

        /* Compute missing terms up to index n. */
        for (idx_t k = size; k < n; ++k) {
            seq_t prev = cache[k-1];                /* a(k) */
            seq_t val  = 6 * (seq_t)k + (seq_t)5;   /* 6*k + 5 */
            seq_t g    = gcd(prev, val);
            seq_t diff = (prev >= g) ? (prev - g) : (g - prev);
            cache[k] = diff;                         /* a(k+1) */
        }
        size = n;
    }

    return cache[n-1];  /* a(n) is stored at index n-1 */
}
