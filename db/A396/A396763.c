/*
 * A396763  Numbers whose average of aliquot divisors and average of divisors
 *          are both integers.
 * https://oeis.org/A396763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 5833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the numbers whose arithmetic mean of all divisors and
   arithmetic mean of the proper divisors are both integers.
   OEIS A396763.
   The function searches sequentially for such numbers, storing
   them in a static cache for fast repeated queries.
   This implementation works for all allowed seq_t and idx_t types. 
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* int_sqrt from utils */
#include "../utils/int_sqrt.h"

static seq_t *cache = NULL;      /* cached terms of the sequence   */
static size_t cached = 0;        /* number of terms cached         */
static size_t cache_cap = 0;     /* allocated capacity for cache   */

static uint64_t *primes = NULL;  /* list of primes used in factoring */
static size_t prime_cnt = 0;     /* number of primes stored         */
static size_t prime_cap = 0;     /* allocated capacity for primes   */

/* --------------------------------------------------------------------- */
/* Dynamic array helpers                                                */
/* --------------------------------------------------------------------- */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

static void ensure_prime_cap(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    uint64_t *new_ptr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    primes = new_ptr;
    prime_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Prime generation                                                    */
/* --------------------------------------------------------------------- */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    primes[prime_cnt++] = p;
}

/* Simple trial division prime generator up to (and including) limit */
static void generate_primes_up_to(uint64_t limit)
{
    if (prime_cnt == 0) {
        add_prime(2);
    }
    uint64_t cand = primes[prime_cnt - 1] + 1;
    if (cand < 3) cand = 3;
    if (cand % 2 == 0) cand++;
    while (cand <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(cand);
        cand += 2;
    }
}

static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt == 0 || primes[prime_cnt - 1] < limit)
        generate_primes_up_to(limit);
}

/* --------------------------------------------------------------------- */
/* Sequence generator                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the OEIS offset */

    /* Return cached value if already available. */
    if ((size_t)n <= cached) return cache[n - 1];

    /* Determine starting candidate: the next integer after the last
       cached term, or 3 if nothing is cached. */
    idx_t candidate = (cached == 0) ? 3 : (idx_t)cache[cached - 1] + 1;

    while ((size_t)n > cached) {
        /* Factorize candidate */
        uint64_t remaining = candidate;
        seq_t tau = 1;    /* number of divisors */
        seq_t sigma = 1;  /* sum of divisors */

        uint64_t limit = int_sqrt(candidate);
        ensure_primes_upto(limit);

        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= remaining; ++i) {
            if (remaining % primes[i] == 0) {
                int exp = 0;
                while (remaining % primes[i] == 0) {
                    remaining /= primes[i];
                    ++exp;
                }
                /* Compute (p^(exp+1)-1)/(p-1) iteratively */
                seq_t p_pow = 1;
                seq_t sum_p = 1;
                for (int j = 0; j < exp; ++j) {
                    p_pow *= (seq_t)primes[i];
                    sum_p += p_pow;
                }
                sigma *= sum_p;
                tau *= (seq_t)(exp + 1);
            }
        }
        if (remaining > 1) {   /* remaining is a prime factor */
            seq_t p = (seq_t)remaining;
            sigma *= (1 + p);
            tau *= 2;
        }

        /* Check average conditions */
        if (sigma % tau == 0) {
            seq_t s = sigma - (seq_t)candidate; /* sum of proper divisors */
            if (tau > 1 && s % (tau - 1) == 0) {
                /* candidate satisfies the definition */
                ensure_cache_cap(cached + 1);
                cache[cached++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }

    return cache[n - 1];
}
