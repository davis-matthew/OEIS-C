/*
 * A325247  Numbers whose omega-sequence is strict (no repeated parts).
 * https://oeis.org/A325247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer whose Ω‑sequence contains no
 *            repeated * values (strict). This implementation builds the
 *            sequence by scanning numbers * and caching all found terms
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   bigomega, factorize, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/factorize.h"
#include "../utils/is_prime.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Red function: product of prime(i) for each exponent i in the factorization of n */
static uint64_t red_of(uint64_t n)
{
    if (n <= 1) return 1;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= get_prime((idx_t)exps[i]);   /* exps[i] >= 1 */
    }
    return prod;
}

/* Test whether the Ω‑sequence of n is strict (no repeated Ω values) */
static bool strict_omega_sequence(uint64_t n)
{
    if (n == 1) return true;
    uint64_t seen = 0;          /* bitmask of Ω values already seen */
    uint64_t cur   = n;
    while (true) {
        unsigned w = (unsigned)bigomega(cur);
        if (seen & ((uint64_t)1 << w)) return false;
        seen |= ((uint64_t)1 << w);
        if (is_prime(cur) || cur == 1) break;
        cur = red_of(cur);
    }
    return true;
}

/* Cache for previously computed terms */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Grow the cache array if needed, using the helper seq_realloc */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */
    size_t target = (size_t)n;            /* n ≥ 1 so target ≥ 1 */

    if (cache_len < target) {
        ensure_capacity(target);
        uint64_t cand = cache_len
                         ? ((uint64_t)cache[cache_len - 1]) + 1ULL
                         : 1ULL;
        while (cache_len < target) {
            if (strict_omega_sequence(cand)) {
                cache[cache_len++] = (seq_t)cand;
            }
            ++cand;
        }
    }
    return cache[target - 1];
}
