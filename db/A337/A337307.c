/*
 * A337307  a(1) = 1; a(n) = 1 + Product_{k=1..n-1} a(k) (mod n-1).
 * https://oeis.org/A337307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337307
 * a(1) = 1;
 * a(n) = 1 + ∏_{k=1}^{n-1} a(k) (mod n-1)   (n ≥ 2) */
#include <stdlib.h>
#include "../utils/alloc_cache.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */

    static seq_t *cache = NULL;           /* a(1) stored at cache[0] */
    static idx_t cache_size = 0;          /* number of values currently cached */

    idx_t target = n - 1;                 /* index in zero‑based array */
    if (target >= cache_size) {
        idx_t new_size = target + 1;
        seq_t *new_cache = alloc_cache(new_size);
        if (!new_cache) abort();

        for (idx_t i = 0; i < new_size; ++i) {
            if (i == 0) {                 /* a(1) */
                new_cache[0] = (seq_t)1;
                continue;
            }
            seq_t prod  = (seq_t)1;
            seq_t modx  = (seq_t)(i);     /* n-1 */

            for (idx_t k = 0; k < i; ++k) {
                seq_t term = new_cache[k] % modx;
                prod = (prod * term) % modx;
            }
            new_cache[i] = prod + (seq_t)1;
        }

        /* discard old cache – acceptable leak for the allowed limits */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[target];
}
