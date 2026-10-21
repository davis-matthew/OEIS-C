/*
 * A257292  Numbers whose square can be written as the sum of two consecutive
 *          nonsquares.
 * https://oeis.org/A257292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A257292: numbers whose square can be written as the sum of
   two consecutive nonsquares.  The sequence consists of the odd
   integers n such that neither ⌊n²/2⌋ nor ⌈n²/2⌉ is a perfect square. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* --------- Internal helper: floor of integer square root for idx_t --------- */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* Binary search implementation that works for any 64‑bit integer. */
    idx_t top = ((idx_t)1 << 62);          /* largest 2⁶² */
    while (top > n) top >>= 2;             /* bring top close to sqrt(n) */
    top <<= 1;                             /* start from a power of two >= sqrt(n) */
    idx_t res = 0;
    while (top != 0) {
        idx_t candidate = res + (top >> 1);
        if (candidate * candidate <= n) {
            res = candidate;
        }
        top >>= 1;
    }
    return res;
}

/* --------- Cached sequence storage --------------------------------------- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;   /* number of terms currently cached   */
static size_t cache_cap = 0;   /* allocated capacity in cache array   */
static idx_t  next_test = 1;   /* next integer to examine (odd only) */

/* Ensure that at least 'need' terms are cached. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    while (cache_len < (size_t)need) {
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
        }

        idx_t cand = next_test++;
        if ((cand & 1ULL) == 0)          /* even numbers cannot satisfy the condition */
            continue;

        idx_t sq = cand * cand;          /* square of the candidate */
        idx_t m1 = sq >> 1;              /* floor(cand²/2)          */
        idx_t m2 = m1 + 1;               /* ceil on the same value   */

        idx_t r1 = floor_sqrt_u64(m1);
        if (r1 * r1 == m1)
            continue;                    /* m1 is a perfect square */

        idx_t r2 = floor_sqrt_u64(m2);
        if (r2 * r2 == m2)
            continue;                    /* m2 is a perfect square */

        cache[cache_len++] = (seq_t)cand; /* store the valid candidate */
    }
}

/* Return the nth term of sequence A257292 (n ≥ 1).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safer return, though the sequence is defined for n ≥ 1 */

    ensure_cache(n);
    return cache[n-1];
}
