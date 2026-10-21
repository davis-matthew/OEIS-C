/*
 * A279756  Smallest prime p >= prime(n) such that p == 2 (mod prime(n)).
 * https://oeis.org/A279756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p >= prime(n) such that p ≡ 2 (mod
 *            prime(n))
 * Range:     u8 n <= 14, u16 n <= 362, u32 n <= 270, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Computes A279756 – see OEIS entry for definition. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache     = NULL;
    static idx_t cache_len = 0;   /* number of entries stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    if (n == 0) return 0;        /* undefined; return 0 for safety */

    /* extend cache if needed */
    if (n > cache_len) {
        /* grow cached storage */
        if (n > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < (size_t)n) new_cap *= 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            uint32_t q = get_prime(i);          /* i‑th prime, uint32_t */
            if (q == 2) {                       /* special case */
                cache[i-1] = 2;
                continue;
            }
            seq_t p = (seq_t)q + 2;             /* first candidate ≥ q with p≡2 (mod q) */
            while (!is_prime_big(p)) {          /* find the next prime in this progression */
                p += q;
            }
            cache[i-1] = p;
        }
        cache_len = n;
    }

    return cache[n-1];
}
