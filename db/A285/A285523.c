/*
 * A285523  Numbers k such that k^2 + 1 is 100-smooth.
 * https://oeis.org/A285523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 137, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the n‑th integer k such that k²+1 is 100‑smooth (all prime
   factors ≤ 97). The function computes terms lazily, storing
   previously found terms in a static cache, so that repeated calls
   (in any order) are fast. */
#include <stdint.h>
#include <stdlib.h>

/* Allowed odd prime factors of k²+1. */
static const unsigned long long primes_100smooth[]
    = {5ULL,13ULL,17ULL,29ULL,37ULL,41ULL,53ULL,61ULL,73ULL,89ULL,97ULL};
static const size_t PRIME_COUNT
    = sizeof(primes_100smooth) / sizeof(primes_100smooth[0]);

/* Static cache of discovered terms. */
static seq_t *seq_cache      = NULL;
static size_t seq_count      = 0;
static size_t seq_capacity   = 0;
static unsigned long long next_candidate = 1;   /* next k to test */

/* Grow the cache array when it runs out of space. */
static void grow_cache(void)
{
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    seq_cache = tmp;
    seq_capacity = new_cap;
}

/* Test whether k satisfies the 100‑smooth condition. */
static int is_100smooth(unsigned long long k)
{
    unsigned __int128 v = (unsigned __int128)k * k + 1;
    if (k & 1U) v >>= 1;                     /* remove the unique factor 2 */
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        unsigned long long p = primes_100smooth[i];
        while (v % p == 0) v /= p;
        if (v == 1) break;
    }
    return v == 1;
}

/* Ensure at least n terms are cached. */
static void ensure_terms(idx_t n)
{
    while (seq_count < (size_t)n) {
        while (!is_100smooth(next_candidate)) ++next_candidate;
        if (seq_count == seq_capacity) grow_cache();
        seq_cache[seq_count++] = (seq_t)next_candidate;
        ++next_candidate;                     /* next search starts after this k */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* below offset, undefined */
    ensure_terms(n);
    return seq_cache[(size_t)n - 1];
}
