/*
 * A325264  Numbers whose omega-sequence sums to 7.
 * https://oeis.org/A325264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   bigomega, build_primes, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A325264: the n‑th integer whose omega‑sequence sums to 7.
   The function generates terms on demand, caching them so that calls
   in any order require only linear work in the number of produced
   terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stddef.h>
#include "../utils/bigomega.h"
#include "../utils/build_primes.h"
#include "../utils/isqrt.h"
#include "../utils/mem.h"

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

/* ------------------------------------------------------------------ */
/*  Apply the red operation defined in A181819.                        */
/* ------------------------------------------------------------------ */
static idx_t
apply_red(idx_t n)
{
    if (n <= 1)
        return 1;

    idx_t r = 1;
    idx_t temp = n;
    idx_t limit = isqrt(n) + 1;
    idx_t *prime_list = NULL;
    size_t prime_cnt = 0;

    /* Generate primes up to sqrt(n) */
    build_primes(limit, &prime_list, &prime_cnt);

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
        if (temp % prime_list[i] == 0) {
            int exp = 0;
            while (temp % prime_list[i] == 0) {
                temp /= prime_list[i];
                ++exp;
            }
            /* Multiply by the exp‑th prime. */
            r *= (idx_t)get_prime((idx_t)exp);
        }
    }
    /* Remaining prime factor has exponent 1. */
    if (temp > 1)
        r *= (idx_t)get_prime(1);

    free(prime_list);
    return r;
}

/* ------------------------------------------------------------------ */
/*  Sum of the omega‑sequence of n.                                   */
/* ------------------------------------------------------------------ */
static seq_t
omega_seq_sum(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    idx_t cur = n;

    while (true) {
        seq_t om = bigomega(cur);
        sum += om;
        if (om == 1)
            break;
        cur = apply_red(cur);
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Cached generation of the sequence.                                */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 2;

static void
ensure_cache_at_least(idx_t n)
{
    while (cache_len < n) {
        seq_t sum = omega_seq_sum(next_candidate);
        if (sum == 7) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_cache = mem_resize(seq_t, seq_cache, cache_cap);
            }
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public function required by the tester.                           */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety for out of bounds */
    ensure_cache_at_least(n);
    return seq_cache[n - 1];
}
