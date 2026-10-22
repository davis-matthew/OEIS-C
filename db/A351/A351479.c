/*
 * A351479  Numbers whose sum of odd digits is twice the sum of even digits.
 * https://oeis.org/A351479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A351479: Numbers whose sum of odd digits
   is twice the sum of even digits.  The terms are
   obtained by a simple exhaustive search over positive
   integers.  The function caches the sequence, so
   calls may come in arbitrary order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Test whether an integer satisfies the defining property. */
static bool satisfies(idx_t v);

/* ---------- Cache storage ---------- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
static idx_t next_val = 1;            /* next integer to test */
#else
static seq_t *cache = NULL;           /* dynamic array of found terms */
static size_t cache_cap = 0;          /* allocated capacity          */
static size_t cache_len = 0;          /* number of terms stored      */
static idx_t next_val = 1;            /* next integer to test */
#endif

/* Make sure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
#ifdef __cplusplus
    while (cache.size() < (size_t)n) {
        if (satisfies(next_val))
            cache.push_back(seq_t(next_val)); /* construct seq_t from idx_t */
        ++next_val;
    }
#else
    while (cache_len < (size_t)n) {
        if (satisfies(next_val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     new_cap * sizeof(seq_t));
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_val;   /* cast to seq_t */
        }
        ++next_val;
    }
#endif
}

/* Return the nth term (1‑based).  Undefined for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
#ifdef __cplusplus
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}

/* Check whether a positive integer satisfies the digit‑sum property. */
static bool satisfies(idx_t v)
{
    unsigned sum_odd = 0;
    unsigned sum_even = 0;
    idx_t tmp = v;
    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        if (d & 1U)
            sum_odd += d;
        else
            sum_even += d;
        tmp /= 10;
    }
    return sum_odd == 2U * sum_even;
}
