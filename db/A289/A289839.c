/*
 * A289839  Primes of the form 8*n^2+8*n+31.
 * https://oeis.org/A289839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A289839: primes of the form 8*n^2+8*n+31 (n≥0).
   a(1) corresponds to n = 0. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache = NULL;          /* cached prime values          */
    static size_t cache_len = 0;         /* number of cached values      */
    static size_t cache_cap = 0;         /* allocated capacity           */
    static idx_t next_k = 0;             /* next n to test (0‑based)     */

    /* a(1) is the first term; 0 is undefined – return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n cached. */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        /* compute 8*k^2 + 8*k + 31 using a wide type to avoid overflow */
        unsigned __int128 vbig = (unsigned __int128)8 * k * k
                               + (unsigned __int128)8 * k
                               + (unsigned __int128)31;
        seq_t val = (seq_t)vbig;         /* cast into seq_t (may wrap if too large) */

        if (is_prime_big(val)) {
            /* enlarge the cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    /* return the requested term (1‑based index) */
    return cache[(size_t)(n - 1)];
}
