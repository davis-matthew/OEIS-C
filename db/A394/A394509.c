/*
 * A394509  Numbers k for which sopf(2*k) = sopf(k + 2) where sopf = A008472.
 * https://oeis.org/A394509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 898, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A394509: numbers k such that sopf(2*k) = sopf(k + 2),
   where sopf is the sum of DISTINCT prime factors (OEIS A001359). */
/* The routine performs an incremental search, caching found terms
   to allow random access to the sequence.  All arithmetic is done
   in 64‑bit unsigned integers before converting to seq_t.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* Sum of distinct prime factors of a 64‑bit integer. */
static seq_t sopf_distinct_uint64(uint64_t n)
{
    seq_t sum = 0;
    uint64_t last = 0;          /* last prime factor seen (for distinctness) */
    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        if (p != last) {
            sum += (seq_t)p;
            last = p;
        }
        n /= p;                  /* divide out this prime factor once */
    }
    return sum;
}

/* Cache for the sequence terms. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 2;   /* next candidate k to test */

/* Return the n‑th term (1‑based).  Random access is supported by caching. */
seq_t a(idx_t n)
{
    /* Grow cache until it contains n elements. */
    while (cache_len < (size_t)n && next_k > 0) {
        uint64_t k = next_k++;

        /* Compute sopf(2*k) and sopf(k+2). */
        uint64_t twok = k << 1;               /* 2*k  (no overflow assumed for needed range) */
        seq_t s2k = sopf_distinct_uint64(twok);
        seq_t sk2 = sopf_distinct_uint64(k + 2);

        if (s2k == sk2) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed; aborting to avoid undefined behaviour. */
                    abort();
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* If n is larger than we could compute (e.g., due to overflow), return 0. */
    if (n == 0 || cache_len < (size_t)n) {
        return 0;
    }
    return cache[(size_t)n - 1];
}
