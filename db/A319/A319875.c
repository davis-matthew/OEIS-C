/*
 * A319875  a(n) is the number of liberties of the group of n stones labeled 1
 *          to n, by analogy to the game of go, in a pi(n)-dimensional board
 *          where a stone's coordinates are its label's p-adic valuations.
 * https://oeis.org/A319875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(p) = a(p-1) + p-1 for prime p; a(c) = a(c-1) + pi(c)
 *            - pi(gpf(c)) for composite c. pi(x) counts primes ≤ x; gpf(x) is
 *            the greatest prime factor
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"
#include "../utils/build_primes_upto.h"

/* Cached sequence values: a_cache[i] = a(i),  i ≥ 0 (index 0 unused).  */
static seq_t *a_cache = NULL;
/* Cached prime‑counting values: pi_cache[i] = π(i),  0 ≤ i ≤ cached_max.  */
static size_t *pi_cache = NULL;
/* List of primes up to the current maximum, used for pi_cache.  */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
/* Highest index for which a_cache and pi_cache have been filled.  */
static size_t cached_max = 0;

/* Ensure that the caches cover at least up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cached_max) return;

    /* New allocation size (include 0).  */
    size_t new_len = (size_t)n + 1;

    /* Resize sequence and prime‑count caches.  */
    a_cache  = (seq_t  *)realloc(a_cache,  new_len * sizeof(seq_t));
    pi_cache = (size_t *)realloc(pi_cache, new_len * sizeof(size_t));

    /* Build primes up to n.  */
    free(prime_list); /* free previous list if any */
    build_primes_upto(n, &prime_list, &prime_cnt);

    /* Compute pi_cache using the new prime list.  */
    size_t p_idx = 0;
    for (size_t i = 0; i <= (size_t)n; ++i) {
        while (p_idx < prime_cnt && prime_list[p_idx] <= i) ++p_idx;
        pi_cache[i] = p_idx;
    }

    /* Compute a_cache entries from cached_max+1 up to n.  */
    for (size_t k = cached_max + 1; k <= (size_t)n; ++k) {
        if (k == 1) {
            a_cache[1] = (seq_t)0;
            continue;
        }
        if (pi_cache[k] > pi_cache[k-1]) {           /* k is prime   */
            a_cache[k] = a_cache[k-1] + (seq_t)(k-1);
        } else {                                    /* k is composite */
            uint64_t g   = gpf((uint64_t)k);          /* gpf(k)       */
            size_t g_pi = pi_cache[g];               /* π(gpf(k)) */
            a_cache[k] = a_cache[k-1] + (seq_t)(pi_cache[k] - g_pi);
        }
    }

    cached_max = (size_t)n;
}

/* External sequence function.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined, return 0 safely */
    ensure_cache(n);
    return a_cache[(size_t)n];
}
