/*
 * A267760  a(n+1) contains as substring the sum of its first digit plus the
 *          last digit of a(n); lexicographic first permutation of the
 *          nonnegative integers with this property.
 * https://oeis.org/A267760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   contains_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A267760:
   a(0) = 0, thereafter a(n+1) is the lexicographically smallest unused
   non‑negative integer whose decimal representation contains the
   substring equal to (first digit of a(n+1)) + (last digit of a(n)).  */

/* Header inclusions required by the build system. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/contains_sum.h"

/* Helper: take most significant digit of a non‑negative integer. */
static inline uint8_t msd(uint64_t x)
{
    if (x == 0) return 0;
    while (x >= 10) x /= 10;
    return (uint8_t)x;
}

/* Dynamic boolean array to remember which numbers have been used. */
static bool  *used      = NULL;
static size_t used_cap  = 0;

/* Ensure `used` has capacity for index `v`. */
static void ensure_used_cap(uint64_t v)
{
    if (v >= used_cap) {
        size_t new_cap = (used_cap ? used_cap * 2 : 16);
        while (new_cap <= v) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) exit(1);
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }
}

/* Cache for the sequence. */
static seq_t  *cache     = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;

/* Ensure `cache` has capacity for index `i`. */
static void ensure_cache_cap(size_t i)
{
    if (i >= cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
        if (new_cap <= i) new_cap = i + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Generate the sequence on demand. */
seq_t a(idx_t n)
{
    /* Lazy initialization of cache and used array. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = 16;
        cache_len = 1;
        cache[0] = (seq_t)0;                 /* a(0) = 0 */
        used = (bool *)calloc(16, sizeof(bool));
        if (!used) exit(1);
        used_cap = 16;
        used[0] = true;
    }

    size_t target = (size_t)n;
    while (cache_len <= target) {
        /* Previous term and its last digit. */
        seq_t   prev        = cache[cache_len - 1];
        uint8_t last_digit  = (uint8_t)(prev % (seq_t)10);

        /* Search for the smallest unused candidate. */
        uint64_t cand = 0;
        for (;; ++cand) {
            ensure_used_cap(cand);
            if (used[cand]) continue;

            uint8_t first = msd(cand);
            int sum = (int)first + (int)last_digit;

            if (contains_sum(cand, sum)) {
                /* Accept this candidate. */
                ensure_used_cap(cand);
                used[cand] = true;
                ensure_cache_cap(cache_len);
                cache[cache_len++] = (seq_t)cand;
                break;
            }
        }
    }
    return cache[target];
}
