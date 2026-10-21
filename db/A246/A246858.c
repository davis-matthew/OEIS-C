/*
 * A246858  Composite numbers k such that sigma(k + sigma(k)) = 2*sigma(k).
 * https://oeis.org/A246858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   ensure_capacity, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246858: composite numbers k such that sigma(k + sigma(k)) = 2*sigma(k) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static idx_t       last_checked = 1;   /* last integer examined */
static size_t      cache_len   = 0;    /* number of terms stored */
static size_t      cache_cap   = 0;    /* capacity of the cache array */
static seq_t      *cache       = NULL; /* cached terms */

seq_t a(idx_t n)
{
    if (n == 0) /* safety – sequence is 1‑based, but callers may pass 0 */
        return 0;

    while (cache_len < n) {
        ++last_checked;

        /* Skip numbers smaller than 2 and primes, keeping only composites */
        if (last_checked < 2 || is_prime(last_checked))
            continue;

        seq_t sigma_k  = sigma_of(last_checked);

        /* Compute k + sigma(k) and its sigma */
        idx_t  target   = last_checked + (idx_t)sigma_k;
        seq_t  sigma_sum = sigma_of(target);
        seq_t  double_sigma = sigma_k + sigma_k;

        if (sigma_sum == double_sigma) {
            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
