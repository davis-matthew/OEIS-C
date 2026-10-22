/*
 * A369938  Numbers whose maximal exponent in their prime factorization is a
 *          power of 2.
 * https://oeis.org/A369938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 223, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_power_of_two_uint64, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369938:  numbers whose maximal exponent in their prime
 * factorization is a power of 2.
 *
 * This implementation builds the list incrementally, caching the terms
 * found so far.  It uses a prime table (built once) to factor each
 * candidate integer; the largest prime‑power exponent is checked
 * for being a power of two.  A static cache of seq_t values holds
 * the sequence terms, growing with seq_realloc for arbitrary‑precision
 * builds and free semantics that are valid in both C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_power_of_two_uint64.h"

/*--- static data -----------------------------------------------------*/
static uint64_t *s_primes      = NULL;   /* prime list */
static size_t   s_prime_cnt    = 0;      /* number of primes in list */

static seq_t   *s_cache        = NULL;   /* cached sequence values      */
static size_t   s_cache_cap    = 0;      /* capacity of the cache (elements) */
static size_t   s_cache_sz     = 0;      /* number of terms stored      */

static uint64_t s_next_candidate = 2;    /* next integer to test */

static uint64_t max_exponent_uint64(uint64_t cand)
{
    uint64_t temp = cand;
    uint64_t max_e = 0;

    for (size_t i = 0; i < s_prime_cnt; ++i) {
        uint64_t p = s_primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e > max_e) max_e = e;
        }
    }
    if (temp > 1 && max_e < 1) {
        /* remaining prime factor with exponent 1 */
        max_e = 1;
    }
    return max_e;
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined; harmless for callers */

    /* build prime table once */
    if (s_primes == NULL) {
        /* 200 000 is comfortably large for n up to about 10 000;
           increase the limit if a larger n is expected. */
        const idx_t PRIME_LIMIT = 200000;
        build_primes_upto(PRIME_LIMIT, &s_primes, &s_prime_cnt);
    }

    /* extend cache until it contains at least n terms */
    while (s_cache_sz < (size_t)n) {
        uint64_t cand = s_next_candidate;
        uint64_t e    = max_exponent_uint64(cand);

        if (is_power_of_two_uint64(e)) {
            /* grow cache if necessary */
            if (s_cache_sz >= s_cache_cap) {
                size_t new_cap = s_cache_cap ? s_cache_cap * 2 : 16;
                seq_t *tmp;
                if (s_cache == NULL) {
                    tmp = seq_alloc(new_cap);
                } else {
                    tmp = seq_realloc(s_cache, s_cache_cap, new_cap);
                }
                if (tmp == NULL) {
                    /* out‑of‑memory: abort; not expected in tests */
                    exit(1);
                }
                s_cache        = tmp;
                s_cache_cap    = new_cap;
            }
            s_cache[s_cache_sz++] = (seq_t)cand;
        }
        ++s_next_candidate;
    }

    return s_cache[(size_t)n - 1];
}
