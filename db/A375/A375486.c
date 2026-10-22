/*
 * A375486  a(n) is the number of integers k between 0 and n such that n OR k
 *          is a prime number (where OR denotes the bitwise OR operator).
 * https://oeis.org/A375486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of k in [0,n] such that (n | k) is prime
 * Range:     u8 n <= 382, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Compute a(n): number of integers k, 0 ≤ k ≤ n, such that n OR k is prime. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of entries stored */

    if (n >= cache_len) {
        /* Grow the cache to accommodate up to n. */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely */
        cache = tmp;

        /* Compute values that are not yet cached. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t count = 0;
            for (idx_t k = 0; k <= i; ++k) {
                if (is_prime(i | k)) {
                    ++count;
                }
            }
            cache[i] = count;
        }
        cache_len = new_len;
    }

    return cache[n];
}
