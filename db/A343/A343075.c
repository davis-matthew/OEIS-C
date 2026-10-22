/*
 * A343075  Digitally delicate square numbers (changing any one decimal digit
 *          always produces a nonsquare).
 * https://oeis.org/A343075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: n-th digitally delicate square number,
   i.e. an integer square that remains a nonsquare after changing
   any single decimal digit. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Cache for found delicately square numbers (zero‑based index). */
static seq_t *delicate_cache   = NULL;
static size_t cache_capacity   = 0;
static size_t cache_length     = 0;

/* Next integer root to test: stores the next integer whose square
   might be a delicately square. */
static idx_t next_root = 1;

/* Ensure the cache has at least 'needed' slots. */
static void ensure_cache_capacity(size_t needed)
{
    if (delicate_cache == NULL) {
        cache_capacity = 32;
        delicate_cache = (seq_t *)malloc(sizeof(seq_t) * cache_capacity);
    }
    while (cache_capacity < needed) {
        size_t new_cap = cache_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(delicate_cache,
                                      new_cap * sizeof(seq_t));
        if (!tmp) abort();
        delicate_cache = tmp;
        cache_capacity = new_cap;
    }
}

/* Fast square test using the provided floor_sqrt helper. */
static bool is_square(seq_t v)
{
    seq_t r = floor_sqrt(v);
    return (r * r) == v;
}

/* Test whether a square number is "digitally delicate". */
static bool is_delicate_square(seq_t s)
{
    /* Pre‑compute powers of ten once. */
    static seq_t pow10[20];
    static bool pow10_ready = false;
    if (!pow10_ready) {
        pow10[0] = 1;
        for (int i = 1; i < 20; ++i)
            pow10[i] = pow10[i - 1] * 10;
        pow10_ready = true;
    }

    seq_t pow = 1;
    while (pow <= s) {
        seq_t cur = (s / pow) % 10;
        for (int d = 0; d < 10; ++d) {
            if ((seq_t)d == cur) continue;
            seq_t newval = s - cur * pow + (seq_t)d * pow;
            if (is_square(newval))
                return false;   /* changed digit produced a square */
        }
        pow *= 10;
    }
    return true;   /* all substitutions produce nonsquares */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return zero as a harmless value. */
    if (n == 0) return 0;

    /* Lazy cache allocation. */
    if (delicate_cache == NULL)
        ensure_cache_capacity(32);

    /* Generate delicate squares until we have at least n elements. */
    while (cache_length < (size_t)n) {
        /* Stop if the square would overflow seq_t. */
        if (next_root > ((idx_t)(seq_t)-1) / next_root) {
            /* No larger squares can fit into seq_t, so stop. */
            break;
        }
        seq_t sq = (seq_t)next_root * (seq_t)next_root;
        if (is_delicate_square(sq)) {
            ensure_cache_capacity(cache_length + 1);
            delicate_cache[cache_length++] = sq;
        }
        ++next_root;
    }

    /* Return the requested term if it exists, otherwise zero. */
    if (n <= cache_length)
        return delicate_cache[n - 1];
    else
        return 0;
}
