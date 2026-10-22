/*
 * A303739  Numbers k such that 9*k^2 + 3*k + 1 (A082040) is prime.
 * https://oeis.org/A303739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = indices k such that 9*k^2 + 3*k + 1 is prime (OEIS
 *            A303739)
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 30, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found indices */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k      = 0;       /* last examined k */

    /* return cached result if already known */
    if (n > 0 && (size_t)n <= cache_len)
        return cache[n - 1];

    /* expand the cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        /* next k to test */
        last_k++;
        seq_t kk = (seq_t)last_k;

        /* compute 9*kk^2 + 3*kk + 1 using seq_t arithmetic */
        seq_t val = (seq_t)9 * kk * kk + (seq_t)3 * kk + (seq_t)1;

        if (is_prime_big(val)) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {        /* allocation failure – abort with 0 */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = kk;
        }
    }

    return cache[n - 1];
}
