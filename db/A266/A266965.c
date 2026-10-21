/*
 * A266965  Primes of the form p = a^2 + b^2 where |a^2 - b^2| is composite.
 * https://oeis.org/A266965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266965: primes p = a² + b² with |a² – b²| composite.
   For each prime candidate p ≡ 1 (mod 4) we try every representation
   a² + b² = p (with a ≥ b).  If any such representation gives a
   composite difference a² – b², the prime is accepted.  The sequence
   is built by incremental search; values are cached so calls in any
   order are handled efficiently. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cache of found terms */
static size_t cache_len = 0;     /* number of elements in cache    */
static size_t cache_cap = 0;     /* allocated capacity              */
static uint64_t next_candidate = 5;   /* next odd integer to test for primality */

/* Ensure cache has room for one more element. */
static void ensure_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *new_buf = seq_realloc(cache, cache_cap, new_cap);
        if (!new_buf) {
            /* Hard failure: terminate program */
            exit(EXIT_FAILURE);
        }
        cache = new_buf;
        cache_cap = new_cap;
    }
}

/* Generate terms until the cache contains at least n elements. */
static void generate_until(idx_t n)
{
    while (cache_len < n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;          /* consider only odd numbers */

        if (!is_prime_big((seq_t)cand))
            continue;                 /* not a prime -> skip */

        /* Only primes ≡ 1 (mod 4) can be expressed as a²+b². */
        if ((cand & 3U) != 1U)
            continue;

        seq_t p   = (seq_t)cand;
        seq_t sqr = floor_sqrt(p);
        idx_t bound = (idx_t)sqr;

        bool ok = false;
        for (idx_t a_v = bound; a_v >= 1; --a_v) {
            seq_t a   = (seq_t)a_v;
            seq_t a2  = a * a;
            if (a2 > p)          /* safety; should not happen */
                continue;

            seq_t b2  = p - a2;
            seq_t b   = floor_sqrt(b2);
            if (b * b != b2)     /* not a perfect square */
                continue;

            /* Ensure a ≥ b to avoid negative differences. */
            if (a2 < b * b)
                continue;

            seq_t diff = a2 - b * b;   /* non‑negative */
            if (!is_prime_big(diff) && diff > 1) {
                ok = true;
                break;
            }
        }

        if (ok) {
            ensure_capacity();
            cache[cache_len++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but never called */
    generate_until(n);
    return cache[n - 1];
}
