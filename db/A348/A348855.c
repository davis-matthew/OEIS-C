/*
 * A348855  a(1) = 1. If a(n) is prime, a(n+1) = 2*a(n) + 1. If a(n) is not
 *          prime, a(n+1) = least prime not already in the sequence.
 * https://oeis.org/A348855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1662, u32 n <= 2841, u64 n <= 8973,
 *            u128 n <= 8973, big any n
 * Helpers:   compare_seq_t, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A348855.
   a(1) = 1.
   For n ≥ 2, let a(n-1) = x.
   If x is prime then a(n) = 2·x + 1.
   Otherwise a(n) = smallest prime not yet appearing in the sequence. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"
#include "../utils/compare_seq_t.h"

/* --- static data --- */
static seq_t *seq_cache = NULL;           /* 1‑based cache, seq_cache[0] unused */
static idx_t seq_last = 0;                /* highest index computed */
static size_t seq_capacity = 0;           /* allocated number of slots */

static seq_t *seen_primes = NULL;         /* primes that have already been emitted */
static size_t seen_count = 0, seen_cap = 0;
static seq_t next_prime_candidate = (seq_t)2;   /* next integer to test as a prime */

/* Ensure seq_cache has space for at least 'need' indices (including 0). */
static void ensure_cache_capacity(idx_t need)
{
    if ((size_t)need + 1 <= seq_capacity) return;
    grow_seq(&seq_cache, &seq_capacity, (size_t)need + 1);
}

/* Ensure seen_primes has room for at least 'need' elements. */
static void ensure_seen_capacity(size_t need)
{
    if (need <= seen_cap) return;
    grow_seq(&seen_primes, &seen_cap, need);
}

/* Return true if 'p' is already stored in seen_primes. */
static bool prime_already_seen(const seq_t *p)
{
    for (size_t i = 0; i < seen_count; ++i)
        if (compare_seq_t(&seen_primes[i], p) == 0)
            return true;
    return false;
}

/* Record that prime 'p' has been seen. */
static void mark_prime_seen(const seq_t *p)
{
    ensure_seen_capacity(seen_count + 1);
    seen_primes[seen_count++] = *p;
}

/* Find the smallest prime that has not yet appeared. */
static seq_t next_unused_prime(void)
{
    seq_t candidate = next_prime_candidate;
    for (;;) {
        if (!is_prime_big(candidate)) {
            candidate = candidate + (seq_t)1;
            continue;
        }
        if (!prime_already_seen(&candidate)) {
            /* remember this prime for future calls */
            mark_prime_seen(&candidate);
            next_prime_candidate = candidate + (seq_t)1;
            return candidate;
        }
        candidate = candidate + (seq_t)1;
    }
}

/* Build the sequence up to index 'n' (inclusive). */
static void build_to(idx_t n)
{
    if (n <= seq_last) return;
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 1];
        seq_t nxt;
        if (is_prime_big(prev))
            nxt = prev * (seq_t)2 + (seq_t)1;
        else
            nxt = next_unused_prime();

        ensure_cache_capacity(i);
        seq_cache[i] = nxt;

        if (is_prime_big(nxt))
            mark_prime_seen(&nxt);

        ++seq_last;
    }
}

/* Public entry point requested by the build system. */
seq_t a(idx_t n)
{
    if (seq_cache == NULL) {                /* first call: initialise */
        ensure_cache_capacity(1);
        seq_cache[1] = (seq_t)1;            /* a(1) = 1, not prime */
        seq_last = 1;
    }
    build_to(n);
    return seq_cache[n];
}
