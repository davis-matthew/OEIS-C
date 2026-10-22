/*
 * A392354  Composite numbers k such that the Chebyshev distance from 1 to k
 *          in the Ulam spiral is not a prime number.
 * https://oeis.org/A392354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth composite number k such that the Chebyshev distance
 *            from 1 to k in the Ulam spiral is not a prime number
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_custom.h"

/* Integer square root: floor(sqrt(x)) for seq_t values */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x;
    while (lo < hi) {
        seq_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 2;            /* next integer to test */
    while (cache_len < n) {
        seq_t k = next_k++;
        if (!is_prime_custom(k)) {          /* k is composite */
            seq_t s = isqrt_seq(k);
            bool is_sq = (s * s == k);
            seq_t r = is_sq ? (s >> 1) : ((s + 1) >> 1); /* Chebyshev distance */
            if (!is_prime_custom(r)) {      /* distance not prime */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) {
                        /* allocation failure – return 0 */
                        return 0;
                    }
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - 1];
}
