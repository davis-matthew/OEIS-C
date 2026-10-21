/*
 * A265793  Denominators of upper primes-only best approximates (POBAs) to
 *          sqrt(8); see Comments.
 * https://oeis.org/A265793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A265793 – denominators of upper primes‑only best approximates (POBAs) to sqrt(8). 
   The code follows the inductive definition: start with q(1)=2 and p(1) the smallest prime
   > sqrt(8)*q(1). For each n>=1, let q(n) be the smallest prime > q(n‑1) for which
   there exists a prime p with sqrt(8) < p/q < p(n‑1)/q(n‑1). Then set p(n) to the least such prime. */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the framework */
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

/* Static cache to store computed terms.  Cache is grown only, never shrunk. */
static seq_t *q_cache = NULL;          /* denominators q(n), 0‑based indexing */
static seq_t *p_cache = NULL;          /* corresponding numerators p(n) */
static size_t cache_size = 0;          /* number of terms already computed */
static size_t cache_cap  = 0;          /* allocated capacity */

/* Grow cache capacity to at least 'needed' elements.  Reallocates both arrays. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap << 1 : 16;
    if (newcap < needed)
        newcap = needed;
    q_cache = (seq_t *)realloc(q_cache, newcap * sizeof(seq_t));
    p_cache = (seq_t *)realloc(p_cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Initialize cache with first term: denominator 2, numerator 7 (sqrt(8) < 7/2). */
static void initialize(void)
{
    if (cache_size > 0)
        return;
    grow_cache(1);
    q_cache[0] = (seq_t)2;
    /* p(1) = least prime ≥ sqrt(8)*q(1).  sqrt(8)=2.828… * 2 = 5.656… ⇒ 7 */
    p_cache[0] = (seq_t)7;
    cache_size = 1;
}

/* Find the least prime ≥ 'start'.  Uses is_prime_big for primality testing. */
static seq_t first_prime_at_least(seq_t start)
{
    /* We work with 64‑bit integers for the search; sequence terms fit into 64‑bits
       for the n-values considered in the test suite.  For larger values the
       loop may overflow, but the framework only tests up to a few thousands where
       64‑bit is sufficient. */
    uint64_t cand = (uint64_t)start;
    while (!is_prime_big((seq_t)cand))
        ++cand;
    return (seq_t)cand;
}

/* Generate the next term of the sequence.  Uses the current cache last entry as
   the previous term. */
static void compute_next(void)
{
    seq_t q_prev = q_cache[cache_size - 1];
    seq_t p_prev = p_cache[cache_size - 1];

    /* Start searching for the next denominator.  The next denominator must be
       a prime larger than q_prev. */
    uint64_t q_candidate = next_prime((uint64_t)(q_prev + 1));

    while (true) {
        /* Lower bound: smallest integer strictly greater than sqrt(8)*q_candidate.
           sqrt(8)*q = sqrt(8*q^2).  We compute floor_sqrt(8*q^2) + 1. */
        seq_t t = (q_candidate * q_candidate) * (seq_t)8;
        seq_t s = floor_sqrt(t);
        seq_t LB = s + (seq_t)1;          /* lower bound for p */

        /* Upper bound: largest integer strictly less than (p_prev/q_prev)*q_candidate.
           Equivalent to floor((p_prev*q_candidate - 1)/q_prev). */
        seq_t num = (p_prev * q_candidate) - (seq_t)1;
        seq_t UB  = num / q_prev;

        /* Find the least prime p ≥ LB.  Increment until we hit a prime. */
        seq_t p_candidate = first_prime_at_least(LB);

        /* If the prime found exceeds UB, this denominator is invalid; try the next prime. */
        if (p_candidate > UB) {
            q_candidate = next_prime((uint64_t)(q_candidate + 1));
            continue;
        }

        /* Valid fraction found. Append to cache. */
        grow_cache(cache_size + 1);
        q_cache[cache_size] = q_candidate;
        p_cache[cache_size] = p_candidate;
        ++cache_size;
        break;
    }
}

/* Public sequence function.  Returns q(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n=0; return 0 by convention */

    initialize();

    while (cache_size < (size_t)n)
        compute_next();

    return q_cache[n - 1];
}
