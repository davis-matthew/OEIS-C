/*
 * A352217  Smallest power of 2 that is one more than a multiple of 2n-1.
 * https://oeis.org/A352217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(order_2(2*n-1)), n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 1546 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here order_2(m) is the smallest k>0 with 2^k ≡ 1 (mod m).  */
/* The routine caches results so that repeated calls are fast. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* undefined below offset */

    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n > cache_size) {
        size_t new_size = n;
        seq_t *new_cache = mem_new(seq_t, new_size);
        if (new_cache == NULL) return 0;           /* allocation failure */
        if (cache != NULL) {
            mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
            mem_free(cache);
        }
        /* initialise new slots to zero */
        mem_set(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }

    /* return cached value if already known */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* compute multiplicative order of 2 modulo (2*n-1) */
    uint64_t mod = 2 * n - 1;          /* odd modulus, fits in uint64_t */
    uint64_t k;

    if (mod == 1) {                    /* special case: 2^1 ≡ 1 (mod 1) */
        k = 1;
    } else {
        uint64_t cur = 1 % mod;
        k = 0;
        do {
            cur = (cur * 2) % mod;
            ++k;
        } while (cur != 1);
    }

    /* a(n) = 2^k */
    seq_t result = ipow_u64_idx(2, k);

    cache[n - 1] = result;
    return result;
}
