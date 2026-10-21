/*
 * A280734  Product_{k=0..prime(n)} k!.
 * https://oeis.org/A280734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..prime(n)} k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   alloc_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n-th prime, with offset 1 (a(1)=2). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_grow.h"

/* Cached sequence values (offset 1, so a(1) is prod_cache[0]) */
static seq_t *prod_cache = NULL;
static size_t prod_cache_cap = 0;
static size_t prod_cache_len = 0;

/* Prime table */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;

/* Current factorial and product up to cur_k */
static seq_t cur_fact = 1;   /* k! for current k (starting 0! = 1) */
static seq_t cur_prod = 1;   /* product_{i=0..cur_k} i! */
static idx_t cur_k = 0;      /* last k processed */

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* only offset 1 is defined */

    /* Grow cache if necessary */
    while (prod_cache_len < n) {
        /* --- find next prime --- */
        uint64_t cand;
        if (primes_len == 0) {
            cand = 2;
        } else {
            cand = primes[primes_len - 1] + 1;
            /* ensure odd after 2 */
            if ((cand & 1ULL) == 0 && cand != 2) cand++;
        }

        /* standard trial division */
        bool is_prime;
        do {
            is_prime = true;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t p = primes[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = false; break; }
            }
            if (!is_prime) {
                cand += (cand == 2 ? 1 : 2);
            }
        } while (!is_prime);

        /* store the new prime */
        primes = (uint64_t *)alloc_grow(primes, &primes_cap, primes_len + 1,
                                        sizeof(uint64_t));
        primes[primes_len++] = cand;

        /* Update factorial and product up to the new prime */
        for (idx_t k = cur_k + 1; k <= (idx_t)cand; ++k) {
            cur_fact *= (seq_t)k;
            cur_prod *= cur_fact;
        }
        cur_k = cand;

        /* Cache the product value for this prime */
        prod_cache = (seq_t *)alloc_grow(prod_cache, &prod_cache_cap,
                                         prod_cache_len + 1, sizeof(seq_t));
        prod_cache[prod_cache_len++] = cur_prod;
    }

    return prod_cache[n - 1];
}
