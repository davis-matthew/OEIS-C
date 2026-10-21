/*
 * A267455  Primes which are a square (mod 39).
 * https://oeis.org/A267455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1626, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A267455 : Primes which are a square (mod 39)  
    The sequence consists of the primes p such that  
    (p|3) = +1 and (p|13) = +1, together with the primes 3 and 13.  
    For a prime p > 13 this is equivalent to p ≡ 1,4,10,16,22 or 25 (mod 39).  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;          /* cache of sequence values */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* allocated capacity */
static uint64_t last_checked = 2;    /* last integer examined for primality */

/* append/update the static cache */
static void extend_cache(idx_t target) {
    while (cache_len < target) {
        /* find next prime > last_checked */
        uint64_t cand = last_checked + 1;
        while (!is_prime_small(cand))
            ++cand;
        last_checked = cand;

        /* decide whether to keep this prime */
        if (cand == 3 || cand == 13 ||
            cand % 39 == 1 || cand % 39 == 4 || cand % 39 == 10 ||
            cand % 39 == 16 || cand % 39 == 22 || cand % 39 == 25) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)cand);
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for 0, but safe */
    if (cache_len < n)
        extend_cache(n);
    return cache[n - 1];
}
