/*
 * A327241  a(1) = 1. Thereafter, if n is prime, a(n) is the next prime after
 *          a(n-1), but written backwards in base 7; if n is composite, a(n)
 *          is the next composite after a(n-1), written backwards in base 7.
 * https://oeis.org/A327241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8726 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: the sequence A327241 is defined recursively:
   a(1) = 1;
   For n > 1:
      if n is prime, a(n) = reverse‑base7( next prime after a(n‑1) );
      if n is composite, a(n) = reverse‑base7( next composite after a(n‑1) );
   The values are kept in base 10 in the output.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utility functions and allocation helpers supplied by the build environment */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_cache(seq_t *p)
{
    mem_free(p);
}

/* reverse_base7_uint64: convert a uint64_t value to its base‑7
   representation written backwards, returned as seq_t. */
static seq_t reverse_base7_uint64(uint64_t v)
{
    uint8_t digits[32];
    size_t dlen = 0;

    if (v == 0)
        digits[dlen++] = 0;

    while (v > 0) {
        digits[dlen++] = v % 7;
        v /= 7;
    }

    seq_t res = 0;
    for (size_t i = 0; i < dlen; ++i)
        res = res * 7 + digits[i];

    return res;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* a: compute term a(n) (1‑based index).  The function memoises results
   in a static cache so that calls may arrive in any order.  The cache
   is grown by doubling capacity using the provided allocator so that
   it works for both simple integer types and arbitrary‑precision
   built‑in types. */
seq_t a(idx_t n)
{
    if (n == 0)           /* defensive: offset is 1, but accept 0 */
        return 0;

    static seq_t *cache = NULL;   /* array of computed terms, 0‑based */
    static size_t cap = 0;        /* current capacity of cache */
    static size_t len = 0;        /* number of values currently stored */

    /* Grow capacity if required */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;

        seq_t *new_arr = seq_alloc_new_array(new_cap);

        /* copy existing data if any */
        for (size_t i = 0; i < len; ++i)
            new_arr[i] = cache[i];

        if (cache)
            free_cache(cache);

        cache = new_arr;
        cap = new_cap;
    }

    /* If already computed up to n, just return the stored value */
    if (n <= len)
        return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i == 1) {                /* a(1) = 1 by definition */
            cache[0] = 1;
            continue;
        }

        bool prime_i = is_prime_seq((idx_t)i);
        seq_t prev_val = cache[i - 2];
        uint64_t cand = (uint64_t)prev_val + 1;

        if (prime_i) {
            while (!is_prime_seq((idx_t)cand))
                ++cand;
        } else {
            while (cand <= 1 || is_prime_seq((idx_t)cand))
                ++cand;
        }

        cache[i - 1] = reverse_base7_uint64(cand);
    }

    len = n;
    return cache[n - 1];
}
