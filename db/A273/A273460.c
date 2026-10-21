/*
 * A273460  Numbers n such that sum of the divisors of n (except 1 and n) is
 *          equal to the product of the digits of n.
 * https://oeis.org/A273460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A273460: numbers n such that sigma(n)-1-n == digit_product(n).
   The function searches integers sequentially and caches the found
   terms, so any order of calls is in O(k) where k is the largest n
   requested. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_product.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 2;   /* start scanning from 2 */

    if (n < 1) return 0;  /* a(0) is undefined */

    /* Keep searching until we have at least n terms cached */
    while (cache_len < n) {
        uint64_t cand = last_checked++;
        seq_t prod = digit_product(cand);

        /* sigma_of returns sum of all divisors of cand */
        seq_t sigma = sigma_of(cand);
        seq_t sum = sigma - (seq_t)1 - (seq_t)cand;   /* exclude 1 and n */

        if (sum == prod) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* memory error – unlikely in tests */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
