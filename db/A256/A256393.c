/*
 * A256393  Start from a(1) = 2, then alternately add either the largest (if n
 *          is even), or the smallest (if n is odd) prime factor of the
 *          preceding term a(n-1) to get a(n).
 * https://oeis.org/A256393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 467, u32 n <= 3761, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   gpf, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; a(2n)=a(2n-1)+gpf(a(2n-1)); a(2n+1)=a(2n)+lpf(a(2n)) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gpf.h"
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of terms stored, a(1)…a(cache_len) */
    static size_t cache_cap = 0;    /* allocated capacity */

    if (n == 0) {
        /* undefined for n=0 – return 0 */
        return (seq_t)0;
    }

    /* initialise upon first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)2;          /* a(1) = 2 */
        cache_len = 1;
    }

    /* enlarge cache if needed */
    if ((size_t)n > cache_len) {
        size_t new_cap = (size_t)n;
        if (new_cap > cache_cap) {
            cache_cap = new_cap;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* compute missing terms */
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[(size_t)i - 2];   /* a(i-1) */
            uint64_t f;
            if ((i & 1ULL) == 0) {               /* even index */
                f = gpf((uint64_t)prev);
            } else {                             /* odd index */
                f = lpf((idx_t)prev);
            }
            cache[(size_t)i - 1] = prev + (seq_t)f;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
