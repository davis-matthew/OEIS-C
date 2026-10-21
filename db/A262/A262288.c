/*
 * A262288  a(0)=0; thereafter add the smallest positive number that is not a
 *          substring.
 * https://oeis.org/A262288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; for n≥1, a(n)=a(n‑1)+k, where k is the smallest positive
 *            integer whose decimal representation does not appear as a
 *            contiguous substring of a(n‑1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms     */
    static size_t cache_cap = 0;   /* allocated capacity in terms */

    /* Ensure cache[0] exists */
    if (cache_len == 0) {
        cache = seq_alloc(1);      /* allocate space for a(0)      */
        cache[0] = (seq_t)0U;      /* a(0) = 0                     */
        cache_len = 1;
        cache_cap = 1;
    }

    if (n == 0U) {
        return cache[0];
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap = new_cap ? new_cap * 2 : 2;
        }

        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;

        /* Compute terms from the first missing index up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];

            /* Convert prev to a decimal string */
            char prev_buf[2048];
            size_t prev_len;
            seq_to_str(prev, prev_buf, &prev_len);
            prev_buf[prev_len] = '\0';

            /* Find the smallest positive integer not in prev_buf */
            idx_t k = 1U;
            char k_buf[32];
            while (1) {
                int k_len =
                    snprintf(k_buf, sizeof(k_buf), "%llu",
                             (unsigned long long)k);
                k_buf[k_len] = '\0';
                if (strstr(prev_buf, k_buf) == NULL) {
                    break;            /* k is missing */
                }
                ++k;
            }

            cache[i] = prev + (seq_t)k;
        }

        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
