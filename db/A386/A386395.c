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
 * Helpers:   floor_sqrt, seq_from_str, seq_to_str
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
#include "../utils/floor_sqrt.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_from_str.h"

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
        cache = (seq_t *)malloc(CACHE_LIMIT * sizeof(seq_t));
        memset(cache, 0, CACHE_LIMIT * sizeof(seq_t));
        initialized = true;
    }

    return min_final_internal((seq_t)n, cache);
}
