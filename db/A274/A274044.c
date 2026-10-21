/*
 * A274044  Squarefree numbers that are the sum of two squares in more than
 *          one way.
 * https://oeis.org/A274044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 4063, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274044: square-free numbers that are the sum of two non‑zero squares in more than one way.
   The implementation tests each integer for
       • square‑free,
       • no prime factor ≡ 3 (mod 4),
       • at least two distinct prime factors ≡ 1 (mod 4),
   and memoises found terms so that any order of calls works. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether a 64‑bit integer satisfies the conditions for A274044.
   The function returns true exactly for squarefree n that have
   at least two distinct prime factors ≡ 1 (mod 4) and no prime
   factors ≡ 3 (mod 4). */
static inline bool qualifies_uint64(uint64_t n)
{
    if (n < 65)                /* 65 is the first term */
        return false;

    size_t count1mod4 = 0;
    uint64_t t = n;

    /* Handle factor 2: a square factor of 2 would make it not square‑free */
    if ((t & 1) == 0) {
        t >>= 1;                 /* remove one factor of 2 */
        if ((t & 1) == 0)         /* a second factor of 2 remained */
            return false;
    }

    /* Trial division for odd primes up to sqrt(t) */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            /* one occurrence of p found; check for a second one (square) */
            t /= p;
            if (t % p == 0)
                return false;     /* p² divides n, not square‑free */

            /* classify the prime factor */
            if ((p & 3) == 1)
                ++count1mod4;    /* p ≡ 1 (mod 4) */
            else
                return false;    /* p ≡ 3 (mod 4) is forbidden */
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (t > 1) {
        if ((t & 3) == 1)
            ++count1mod4;
        else
            return false;        /* remaining prime ≡ 3 (mod 4) */
    }

    return count1mod4 >= 2;       /* need at least two 1‑mod‑4 primes */
}

/* Static memoisation state */
static seq_t  *seq_cache          = NULL;
static size_t seq_len             = 0;
static size_t seq_cap             = 0;
static uint64_t next_to_test      = 1;

/* Return the n‑th term of A274044 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Ensure n is representable as a size_t (the sequence is small compared to SIZE_MAX) */
    size_t target = (size_t)n;

    /* Lazily extend the cached list until we have at least n terms */
    while (seq_len < target) {
        ++next_to_test;                      /* test next integer */
        if (qualifies_uint64(next_to_test)) {
            /* Grow the cache if necessary */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;   /* start with 8, then double */
                seq_cache = (seq_t *)realloc(seq_cache,
                        seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_to_test;
        }
    }
    return seq_cache[target - 1];
}
