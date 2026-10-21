/*
 * A243655  Positive numbers that are primitively represented by the
 *          indefinite quadratic form x^2 - 3y^2 of discriminant 12.
 * https://oeis.org/A243655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243655: positive numbers primitively represented by the
 * indefinite quadratic form x^2 - 3y^2 of discriminant 12.
 * The function builds the sequence consecutively and memoises the
 * results. Each term a(n) is returned by index n (n ≥ 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

static seq_t *cache   = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* next integer to be tested for representation */
static idx_t next_candidate = 1;

/* Append a value to the cache, growing it if necessary. */
static void append_value(seq_t val)
{
    if (cache_size == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – terminate */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
    }
    cache[cache_size++] = val;
}

/* Test whether n has a primitive representation   x^2 - 3y^2 = n
 * with gcd(x,y) = 1.  The special case n==1 is handled separately
 * because it is represented by x=±1, y=0, which satisfies
 * primitivity. */
static bool is_primitive_represented(seq_t n)
{
    if (n == (seq_t)1) {
        return true;
    }

    for (idx_t y = 1;; ++y) {
        seq_t y2    = (seq_t)y * (seq_t)y;
        seq_t three = y2 * (seq_t)3;
        if (three > n) {
            break;
        }
        seq_t s = n + three;
        seq_t x = floor_sqrt(s);
        if (x * x != s) {
            continue;
        }
        seq_t g = gcd(x, (seq_t)y);
        if (g == (seq_t)1) {
            return true;
        }
    }
    return false;
}

/* Public interface: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;  /* undefined for n=0, guard */
    }

    /* Extend the cache until it contains at least n values. */
    while (cache_size < n) {
        seq_t candidate = (seq_t)next_candidate;
        if (is_primitive_represented(candidate)) {
            append_value(candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];   /* offset 1 */
}
