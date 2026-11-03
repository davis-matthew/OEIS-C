/*
 * A386395  Smallest final number reachable from n by successively replacing a
 *          substring of digits with its square root.
 * https://oeis.org/A386395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9547 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386395: compute the smallest final number reachable from n
 * by repeatedly replacing a square substring (no leading zero)
 * by its integer square root.  The implementation performs
 * depth‑first search with memoisation for numbers below a fixed
 * threshold.  It works for any seq_t supported by the utilities. */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

#define CACHE_BITS 21                 /* 2^21 = 2097152 entries  */
#define CACHE_LIMIT ((size_t)1 << CACHE_BITS)

/* Recursive helper: smallest final number for a given value.
 * The cache holds results for all values < CACHE_LIMIT. */
static seq_t min_final_internal(seq_t val, seq_t *cache) {
    if (val < CACHE_LIMIT) {
        seq_t cached = cache[(idx_t)val];
        if (cached != 0)
            return cached;
    }

    /* Convert to string for substring enumeration. */
    char str[128];
    size_t len = 0;
    seq_to_str(val, str, &len);

    seq_t best = val;
    bool moved = false;

    /* Enumerate all substrings that do not start with ‘0’ */
    for (size_t i = 0; i < len; ++i) {
        if (str[i] == '0')
            continue;                     /* leading zero not allowed */

        uint64_t sub_val = 0;
        for (size_t j = i; j < len; ++j) {
            sub_val = sub_val * 10 + (uint64_t)(str[j] - '0');

            /* Check if sub_val is a perfect square */
            seq_t sqrt_val = floor_sqrt((seq_t)sub_val);
            if ((seq_t)(sqrt_val * sqrt_val) == (seq_t)sub_val) {
                /* Replacement would not change the number? Skip */
                if ((seq_t)sub_val == sqrt_val)
                    continue;

                moved = true;

                /* Build new number string */
                char newbuf[256];
                size_t p = 0;

                /* prefix */
                memcpy(newbuf, str, i);
                p += i;

                /* root digits */
                char rootbuf[64];
                size_t rootlen = 0;
                seq_to_str(sqrt_val, rootbuf, &rootlen);
                memcpy(newbuf + p, rootbuf, rootlen);
                p += rootlen;

                /* suffix */
                memcpy(newbuf + p, str + j + 1, len - j - 1);
                p += len - j - 1;
                newbuf[p] = '\0';

                seq_t new_val = seq_from_str(newbuf);
                seq_t cand = min_final_internal(new_val, cache);
                if (cand < best)
                    best = cand;

                if (best == (seq_t)1) {       /* cannot get below 1 */
                    if (val < CACHE_LIMIT)
                        cache[(idx_t)val] = (seq_t)1;
                    return (seq_t)1;
                }
            }
        }
    }

    if (!moved)                 /* no legal moves */
        best = val;

    if (val < CACHE_LIMIT)
        cache[(idx_t)val] = best;

    return best;
}

/* Main entry point for the OEIS sequence */
seq_t a(idx_t n) {
    /* Allocate and initialise cache on first call */
    static seq_t *cache = NULL;
    static bool initialized = false;

    if (!initialized) {
        cache = mem_new(seq_t, CACHE_LIMIT);
        mem_set(cache, 0, CACHE_LIMIT * sizeof(seq_t));
        initialized = true;
    }

    return min_final_internal((seq_t)n, cache);
}
