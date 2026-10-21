/*
 * A291180  Numbers of the form 4*k + 1 with k >= 1 that are not divisible by
 *          any prime factor of the form 4*m + 1, except themselves.
 * https://oeis.org/A291180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer of the form 4·k+1 (k≥1) * whose prime
 *            divisors are all 3 mod 4, except that a prime * ≡1 mod 4 itself
 *            is allowed. The code builds terms on demand, * caching them for
 *            repeated use
 * Range:     u8 n <= 47, u16 n <= 7774, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Helpers provided by the framework */
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq_array.h"

/* cache for computed terms ------------------------------------- */
static seq_t *cache = NULL;        /* array of values */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  cache_count = 0;     /* number of elements actually stored */

/* next k to test for a new candidate 4·k+1 ---------------------- */
static idx_t next_k = 1;

/* prime sieve state --------------------------------------------- */
static uint64_t *primes = NULL;    /* list of primes up to prime_limit */
static size_t   prime_count = 0;   /* number of primes stored */
static uint64_t prime_limit = 0;   /* maximum prime stored */

/* integer square‑root, binary search -------------------------------- */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid <= n / mid) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* make sure primes up to 'limit' are available ------------------- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;

    if (primes != NULL) {
        free(primes);
    }

    build_primes_upto((idx_t)limit, &primes, &prime_count);
    prime_limit = limit;
}

/* make sure the cache can hold at least 'needed' elements -------- */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;

    grow_seq_array(&cache, &cache_cap, new_cap);
}

/* test a candidate 4·k+1 ---------------------------------------- */
static bool candidate_ok(uint64_t cand)
{
    /* cand ≡ 1 (mod 4) by construction */
    uint64_t rem = cand;

    /* need primes up to sqrt(rem) */
    uint64_t bound = isqrt_uint64(rem);
    if (bound > prime_limit) {
        ensure_primes_upto(bound);
    }

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > rem / p) break;          /* p > sqrt(rem) */

        if (rem % p == 0) {
            if (p % 4 == 1) {
                /* any 1‑mod‑4 factor (except possibly the whole number)
                   disqualifies the candidate */
                return false;
            }
            /* strip all powers of p */
            while (rem % p == 0) rem /= p;
        }
    }

    if (rem > 1) {
        /* remaining prime factor > sqrt(original)
           is allowed only if it equals the whole number */
        if ((rem % 4 == 1) && (rem != cand)) return false;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/* public function: return a(n) ------------------------------------- */
seq_t a(idx_t n)
{
    /* generate terms until we reach index n */
    while (cache_count < n) {
        uint64_t cand = 4ULL * next_k + 1ULL;

        if (candidate_ok(cand)) {
            ensure_cache_cap(cache_count + 1);
            cache[cache_count++] = (seq_t)cand;
        }
        ++next_k;
    }

    return cache[n - 1];
}
