/*
 * A280389  Odd semiprimes that can be represented as 2p+3q, where p and q are
 *          primes.
 * https://oeis.org/A280389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for odd semiprimes that can be represented as 2p+3q, where
   p and q are primes. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"

/* Cache of found terms. */
static seq_t *cache = NULL;
static idx_t cache_cnt = 0;
static size_t cache_cap = 0;

/* Array of primes for candidate q in the 3q part. */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Next odd integer to test. */
static idx_t next_candidate = 3;

/* Append a value to the cache, resizing if necessary. */
static void append_cache(seq_t val)
{
    if (cache_cnt == cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_cnt++] = val;
}

/* Ensure that the prime list covers all primes <= limit. */
static void extend_primes_to(idx_t limit)
{
    if (limit <= primes_limit) return;
    free(primes);
    size_t cnt;
    uint64_t *p;
    if (limit < 2) limit = 2;               /* build_primes_upto needs at least 2 */
    build_primes_upto(limit, &p, &cnt);
    primes = p;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* Generate terms up to index n (1‑based). */
static void generate_terms_up_to(idx_t n)
{
    while (cache_cnt < n) {
        idx_t cand = next_candidate;
        /* Ensure prime list for q up to cand/3. */
        extend_primes_to(cand / 3);

        uint64_t cand64 = (uint64_t)cand;
        uint64_t spf = _smallest_prime_factor(cand64);
        if (spf != cand64) {                 /* composite */
            uint64_t q = cand64 / spf;
            if ((spf & 1U) && (q & 1U) && is_prime((idx_t)q)) {
                /* odd semiprime found; check representation 2p+3q */
                bool representable = false;
                for (size_t i = 0; i < primes_cnt && primes[i] <= cand / 3; ++i) {
                    uint64_t qp = primes[i];
                    uint64_t rem = cand64 - 3 * qp;
                    if (rem < 4) continue;   /* need p' >= 2 */
                    if ((rem & 1U) == 0) {
                        uint64_t pp = rem >> 1;  /* divide by 2 */
                        if (pp >= 2 && is_prime((idx_t)pp)) {
                            representable = true;
                            break;
                        }
                    }
                }
                if (representable)
                    append_cache((seq_t)cand64);
            }
        }
        next_candidate += 2;                  /* next odd number */
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined for n<1, but safety */
    if (cache_cnt < n)
        generate_terms_up_to(n);
    return cache[n - 1];
}
