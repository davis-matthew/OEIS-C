/*
 * A336592  Numbers k such that k/A008835(k) is cubefree, where A008835(k) is
 *          the largest 4th power dividing k.
 * https://oeis.org/A336592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9530 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336592: Numbers k such that k/A008835(k) is cubefree
 * (equivalently, in the prime factorisation of k no exponent is ≡ 3 (mod 4)).
 * Implementation: iterate natural numbers, test each candidate by
 * trial‑division and keep the first n satisfying the condition.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>   /* for size_t */
#include "../utils/mem.h"

/* Test whether a natural number k satisfies the condition. */
static bool is_valid_idx(idx_t k)
{
    idx_t n = k;
    /* trial division by all integers up to √n */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned int cnt = 0;
            while (n % p == 0) {
                n /= p;
                ++cnt;
            }
            if (cnt % 4 == 3)   /* exponent 3 (mod 4) → not cubefree quotient */
                return false;
        }
    }
    /* Remaining factor n ≤ original k, if >1 its exponent is 1 ≡ 1 (mod 4) */
    return true;
}

/* Cached sequence values */
static seq_t *cache      = NULL;   /* dynamic array of values */
static idx_t cache_size  = 0;      /* number of elements in cache         */
static idx_t cache_cap   = 0;      /* current allocation capacity         */
static idx_t next_k      = 1;      /* next integer to test                */

/* Grow cache to hold at least new_cap elements */
static void grow_cache(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* Double the capacity until it exceeds new_cap */
    if (!cache_cap) cache_cap = 8;
    while (cache_cap < new_cap) {
        if (cache_cap > ((size_t)-1) / 2)   /* protect against overflow */
            cache_cap = new_cap;
        else
            cache_cap <<= 1;
    }
    cache = mem_resize(seq_t, cache, cache_cap);
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache */
    while (cache_size < n) {
        if (is_valid_idx(next_k)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
