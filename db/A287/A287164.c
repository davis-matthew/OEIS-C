/*
 * A287164  Primes having a unique partition into three squares.
 * https://oeis.org/A287164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation computes the terms of OEIS A287164.
 * The sequence consists of the primes that have exactly one unordered
 * representation as a sum of three squares:
 *
 *   p = a^2 + b^2 + c^2   with 0 ≤ a ≤ b ≤ c
 *
 * The algorithm keeps a cache of the first found terms and a cache of
 * primes.  Each time the function is called it advances the caches until
 * the requested index is available.  It works for any build in which
 * seq_t is an unsigned integer type (fixed‑width or arbitrary precision)
 * and idx_t is a 64‑bit unsigned integer.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* ------------------- Cache for the sequence ------------------- */
static seq_t *seq_cache = NULL;
static size_t  seq_cap   = 0;
static idx_t   seq_len   = 0;

/* ------------------- Cache for primes ------------------------- */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap     = 0;
static size_t   prime_cnt     = 0;
static uint64_t next_candidate = 2;   /* start with the first odd prime */

/* Append a prime to the prime cache, growing if necessary */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 64 : prime_cap << 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

/* Generate the next prime by trial division using the cached primes */
static uint64_t next_prime(void)
{
    for (;;) {
        uint64_t cand = next_candidate;
        /* advance candidate: after 2 jump to 3, then step by 2 (odd numbers only) */
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;

        seq_t   root_se = floor_sqrt((seq_t)cand);
        uint64_t root   = (uint64_t)root_se;
        bool ok = true;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= root; ++i) {
            if (cand % prime_list[i] == 0) { /* composite */
                ok = false;
                break;
            }
        }
        if (ok) {
            add_prime(cand);
            return cand;
        }
    }
}

/* Check whether a prime p has exactly one unordered representation
 * as a^2 + b^2 + c^2 with 0 ≤ a ≤ b ≤ c. */
static bool unique_three_squares(seq_t p)
{
    /* Primes congruent to 7 (mod 8) are not representable as a sum of three squares. */
    if ((p & 7) == 7) return false;

    seq_t a_limit = floor_sqrt(p);          /* a ≤ sqrt(p) */
    int    cnt    = 0;

    for (seq_t a = 0; a <= a_limit; ++a) {
        seq_t a2  = a * a;
        seq_t rem = p - a2;
        if (rem < 0) break;                 /* no more solutions */

        seq_t b_max = floor_sqrt(rem);       /* b ≤ sqrt(rem) */
        seq_t sqrt_half = floor_sqrt(rem / 2); /* restrict to b^2 ≤ rem/2  →  b ≤ sqrt(rem/2) */
        if (sqrt_half < b_max) b_max = sqrt_half;
        if (b_max < a) continue;

        for (seq_t b = a; b <= b_max; ++b) {
            seq_t b2 = b * b;
            seq_t c2 = rem - b2;
            if (c2 < b2) break;              /* then c < b, no valid ordering */
            seq_t c = floor_sqrt(c2);
            if (c * c == c2 && c >= b) {
                ++cnt;
                if (cnt > 1) return false;   /* more than one representation */
            }
        }
    }
    return cnt == 1;
}

/* Append a new term to the sequence cache, growing if necessary */
static void append_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 32 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Ensure we have computed up to index n (1‑based) */
    while (seq_len < n) {
        uint64_t p = next_prime();   /* next prime */
        /* Skip primes that cannot be represented as three squares */
        if (unique_three_squares((seq_t)p))
            append_term((seq_t)p);
    }
    return seq_cache[n - 1];
}
