/*
 * A284412  a(1) = 1, a(2) = 2; a(n) is the largest prime <= (a(a(n-1)) +
 *          a(n-a(n-1))) for n > 2.
 * https://oeis.org/A284412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; a(n) = largest prime ≤ (a(a(n‑1)) +
 *            a(n‑a(n‑1))) for n > 2
 * Range:     u8 n <= 503, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Ensure the cache can hold the requested index */
    size_t nn = (size_t)n;
    if (nn >= cache_size) {
        size_t new_size = cache_size > 0 ? cache_size : 16;
        while (new_size <= nn) new_size <<= 1;

        seq_t *tmp = (cache_size == 0) ?
                         seq_alloc(new_size) :
                         seq_realloc(cache, cache_size, new_size);

        if (!tmp) { /* allocation failure */
            /* In a real program you would handle this gracefully. */
            while (true) { } /* unreachable */
        }

        /* Zero newly allocated elements (they are zero‑valued). */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;

        /* Initialise the only defined terms if the cache was just created. */
        if (cache == NULL) {
            tmp[1] = 1;
            tmp[2] = 2;
        }
        cache      = tmp;
        cache_size = new_size;
    }

    if (cache[nn] != 0)        /* Already computed */
        return cache[nn];

    /* Recursive computation with memoisation */
    seq_t prev      = a(n - 1);                     /* a(n‑1)   */
    seq_t inner     = a((idx_t)prev);               /* a(a(n‑1)) */
    idx_t second_idx = n - (idx_t)prev;             /* n‑a(n‑1)  */
    seq_t inner2    = a(second_idx);                /* a(n‑a(n‑1)) */
    seq_t sum        = inner + inner2;

    /* Find the largest prime ≤ sum */
    seq_t candidate = sum;
    while (!is_prime_big(candidate)) {
        /* The sequence only takes positive values, so condition below is safe. */
        if (candidate == 0) break;
        --candidate;
    }

    cache[nn] = candidate;
    return candidate;
}
