/*
 * A348955  a(1) = 1; a(n) = Sum_{d|n, d <= sqrt(n)} a(d)^2.
 * https://oeis.org/A348955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= sqrt(n)} a(d)^2 (OEIS A348955)
 * Range:     u8 n <= 1439, u16 n <= 8446, u32 n <= 8446, u64 n <= 8446,
 *            u128 n <= 8446, big any n
 * Helpers:   isqrt_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(1) = 1                                           */
/* The implementation uses a static cache that is grown as needed.
 * It thus works in any call order and is efficient for n up to ~10⁴. */

#include <stdlib.h>   /* malloc, realloc, free, abort, size_t */
#include <stdint.h>   /* uint64_t etc. */
#include "../utils/isqrt_idx_t.h"

/*--- Cache handling -----------------------------------------------------*/
static seq_t *cache = NULL;    /* cache[0] unused, cache[1] = 1          */
static idx_t cache_size = 0;   /* allocated size (elements)             */
static idx_t max_computed = 0; /* largest index for which a(n) stored   */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        /* realloc returns a void*, cast explicitly for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();                     /* memory allocation failure */
        /* If the array grew, initialise new entries to 0 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/*--- Main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1 */

    /* Grow cache if necessary and compute missing values. */
    if (max_computed < n) {
        ensure_cache_up_to(n);

        /* Initialise the first entry if this is the first call. */
        if (max_computed == 0) {
            cache[1] = 1;
            max_computed = 1;
        }

        /* Compute a(m) for each new index m. */
        for (idx_t m = max_computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            idx_t limit = isqrt_idx_t(m);   /* floor(sqrt(m)) */
            for (idx_t d = 1; d <= limit; ++d) {
                if (m % d == 0) {           /* d is a divisor of m */
                    seq_t v = cache[d];
                    sum += v * v;
                }
            }
            cache[m] = sum;
        }
        max_computed = n;
    }
    return cache[n];
}
