/*
 * A345471  a(0) = a(1) = 1, a(n) is the smallest positive integer m >= a(n-1)
 *          + a(n-2) such that gcd(a(k),m) = 1 for all 1 < k <= n - 1.
 * https://oeis.org/A345471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; for n ≥ 2, a(n) is the smallest integer m ≥
 *            a(n‑1)+a(n‑2) such that gcd(a(k), m) = 1 for all 2 ≤ k ≤ n‑1
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;                /* memoised terms of the sequence   */
static size_t cache_cap = 0;               /* allocated elements (capacity)    */
static size_t cache_len = 0;               /* computed terms 0 .. cache_len-1   */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 2) new_cap = 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to and including n. */
    while (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        } else if (cache_len == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t cand = cache[cache_len-1] + cache[cache_len-2];  /* lower bound */
            for (;;) {
                bool coprime = true;
                for (size_t k = 2; k < cache_len; ++k) {
                    if (gcd(cache[k], cand) != (seq_t)1) {
                        coprime = false;
                        ++cand;
                        break;               /* restart the inner check */
                    }
                }
                if (coprime) break;           /* found minimal m */
            }
            cache[cache_len] = cand;
        }
        ++cache_len;
    }

    return cache[(size_t)n];
}
