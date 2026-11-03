/*
 * A342675  a(n) = Sum_{d|n} d^(n-d+1).
 * https://oeis.org/A342675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d+1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 596 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* <big_pow> is provided in the build utilities */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}
/* <ensure_cache> helper to grow a static cache array */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    if (n == 0) return 0;            /* offset is 1, caller must use n>=1 */

    ensure_cache(n, &cache, &cache_cap);
    if (cache[n - 1] != 0) return cache[n - 1];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* term for divisor d */
            seq_t t = ipow((seq_t)d, (seq_t)(n - d + 1));
            sum += t;
            idx_t e = n / d;
            if (e != d) {
                seq_t t2 = ipow((seq_t)e, (seq_t)(n - e + 1));
                sum += t2;
            }
        }
    }
    cache[n - 1] = sum;
    return sum;
}
