/*
 * A242256  Primes that are not primes-greedy summable, as defined at A242252.
 * https://oeis.org/A242256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A242256 (primes that are NOT primes‐greedy summable) by
 * generating primes on demand and testing each with the greedy
 * subtraction algorithm.  No hard‑coded table is used. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* static storage for primes and the resulting sequence */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static idx_t     prime_limit  = 0;

static seq_t *seq_cache  = NULL;
static size_t seq_len    = 0;
static size_t seq_cap    = 0;
static size_t test_idx   = 0;

/* Ensure that prime_list contains all primes ≤ limit.  Reallocate
 * and rebuild the list if a larger limit is requested. */
static void
ensure_prime_limit(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Binary search for a prime value in prime_list.
 * Returns true iff x is in the list. */
static bool
is_prime_in_list(uint64_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t p = prime_list[mid];
        if (p == x)
            return true;
        if (p < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* For a given remainder r (>1) return the largest prime < r.
 * If none exist (i.e. r == 2), returns 0. */
static uint64_t
largest_prime_lt(uint64_t r)
{
    if (r <= 2)
        return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < r)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index with prime_list[lo] >= r; desired is lo-1 */
    if (lo == 0)
        return 0;
    return prime_list[lo - 1];
}

/* Check whether a prime p is greedy‑summable according to
 * the definition in the OEIS entry.  Returns true if p is
 * greedy‑summable, false otherwise. */
static bool
is_greedy_summable(uint64_t p)
{
    if (p == 2)
        return false;          /* 2 is handled separately as non‑summable */

    uint64_t rem = p;
    while (rem > 1) {
        uint64_t sub = largest_prime_lt(rem);
        if (sub == 0)          /* no smaller prime to subtract */
            break;
        rem -= sub;
        if (is_prime_in_list(rem))
            return true;       /* reached a prime remainder -> summable */
    }
    return false;               /* not greedy‑summable */
}

/* Ensure seq_cache has space for need elements. */
static void
ensure_seq_cache(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate terms of the sequence up to index n (1‑based). */
static void
generate_up_to(idx_t n)
{
    /* starting upper bound for prime generation; enlarge if needed */
    const idx_t INITIAL_LIMIT = 2000000;
    if (prime_limit == 0)
        ensure_prime_limit(INITIAL_LIMIT);

    ensure_seq_cache((size_t)n);

    while (seq_len < (size_t)n) {
        if (test_idx >= prime_cnt) {
            /* extend prime list if exhausted */
            ensure_prime_limit(prime_limit << 1);
        }
        uint64_t p = prime_list[test_idx++];
        if (!is_greedy_summable(p))
            seq_cache[seq_len++] = (seq_t)p;
    }
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but safe */
    generate_up_to(n);
    return seq_cache[n - 1];
}
