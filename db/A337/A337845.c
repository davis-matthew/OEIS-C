/*
 * A337845  Primes with a digital sum that is less than the digital sum of the
 *          previous prime.
 * https://oeis.org/A337845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8795 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A337845: primes whose digit sum is less
   than the digit sum of the previous prime (starting from 2). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Helper: digit sum of a 64‑bit integer, result fits in idx_t. */
static idx_t digit_sum_idx(idx_t x)
{
    idx_t s = 0;
    while (x != 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* Static cache of qualifying primes. */
    static seq_t *cache = NULL;   /* array of qualifying primes */
    static size_t cache_cap = 0;  /* capacity of cache */
    static size_t cache_len = 0;  /* number of elements stored */

    /* State while iterating primes. */
    static uint64_t last_prime = 2;    /* last prime examined */
    static idx_t    prev_dsum   = 0;    /* digit sum of previous prime */
    static bool     init_done   = false;

    /* Initialize state on first call. */
    if (!init_done) {
        prev_dsum  = digit_sum_idx(last_prime); /* digit sum of 2 = 2 */
        init_done  = true;
        cache_cap  = 16;
        cache      = alloc_seq(cache_cap);
    }

    /* Ensure cache has at least n elements. */
    while (cache_len < n) {
        /* Find next prime after last_prime. */
        uint64_t cand = last_prime + 1;
        for (;;) {
            if (is_prime(cand))
                break;
            ++cand;
        }

        /* Digit sum of this candidate. */
        idx_t dsum = digit_sum_idx(cand);

        /* If digit sum is strictly less than that of the previous
           prime, store it in the cache. */
        if (dsum < prev_dsum) {
            if (cache_len + 1 > cache_cap) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = (seq_t)cand;
        }

        /* Update state for next iteration. */
        last_prime = cand;
        prev_dsum  = dsum;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
