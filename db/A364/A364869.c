/*
 * A364869  Numbers k such that 6*k+1 is the norm of an Eisenstein prime.
 * https://oeis.org/A364869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 3687, u64 n <= 10000,
 *            u128 n <= 7633, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A364869: Numbers k such that 6*k+1 is prime or a square of a prime ≡ 5 (mod 6). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of elements stored */
    static size_t cache_cap = 0;      /* allocated capacity */
    static idx_t next_k = 1;          /* next k to test for inclusion */

    /* Ensure the cache has space for at least one more element. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure; unlikely in test */
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t p = 6 * (seq_t)next_k + 1;
        bool ok = false;

        if (is_prime_big(p)) {
            ok = true;
        } else {
            seq_t r = floor_sqrt(p);
            if (r * r == p && is_prime_big(r) && (r % 6 == 5)) {
                ok = true;
            }
        }

        if (ok) {
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
