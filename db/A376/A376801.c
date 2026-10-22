/*
 * A376801  a(0) = 397; a(n+1) = a(n)^2 if a(n) is prime, floor(a(n)/2)
 *          otherwise.
 * https://oeis.org/A376801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=397; a(n+1) = a(n)^2 if a(n) is prime, a(n+1) =
 *            floor(a(n)/2) otherwise. The function memoises previously
 *            computed values and works for any external linkage and any build
 *            of seq_t (unsigned integer or arbitrary‑precision class)
 * Range:     u8 none, u16 n <= 0, u32 n <= 61, u64 n <= 90, u128 n <= 99,
 *            big any n
 * Helpers:   is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;   /* array of seq_t */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t cache_len = 0;   /* highest index computed + 1 */

    /* Ensure we have computed up to index n. */
    if (cache_len <= n) {
        /* Allocate or grow the cache as needed. */
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* In an evaluation environment we can abort on OOM. */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Initialise the sequence if this is the first call. */
        if (cache_len == 0) {
            cache[0] = (seq_t)397;
            cache_len = 1;
        }

        /* Compute terms up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            if (is_prime_big(prev))
                cache[i] = seq_mul(prev, prev);
            else
                cache[i] = prev / 2;   /* floor division by 2 */
        }

        cache_len = n + 1;
    }

    return cache[n];
}
