/*
 * A259462  From higher-order arithmetic progressions.
 * https://oeis.org/A259462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259462: From higher‑order arithmetic progressions
 *
 * Formula: a(0) = 1
 * Recurrence: for n ≥ 1
 *     a(n) = ((n+4)*(n+3)*(n+2)^2)
 *            -------------------  * a(n-1)
 *                6 * n
 *
 * The implementation keeps a cache of already‑computed terms.
 * For each new term it multiplies a(n‑1) by the four numerator factors,
 * reducing gcd with the denominator after each step to keep intermediate
 * values small (and avoid overflow on 64‑bit builds).
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* Cache of computed terms */
static idx_t cur_size = 0;    /* Number of valid entries in cache */
static size_t cap = 0;        /* Allocated capacity */

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cap) return;
    size_t new_cap = cap ? cap * 2 : 1;
    while (new_cap < required) new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr) exit(1);          /* allocation failure – abort */
    cache = newptr;
    cap = new_cap;
}

/* Public function: returns a(n) for the sequence A259462. */
seq_t a(idx_t n)
{
    /* Handle the first term. */
    if (n == 0) {
        if (cur_size == 0) {
            ensure_cache(0);
            cache[0] = (seq_t)1;
            cur_size = 1;
        }
        return cache[0];
    }

    /* Ensure cache capacity. */
    ensure_cache(n);

    /* Initialize cache if this is the first call for n > 0. */
    if (cur_size == 0) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        cur_size = 1;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cur_size; i <= n; ++i) {
        seq_t v = cache[i - 1];
        seq_t denom = (seq_t)(6 * i);          /* 6 * n      */

        /* Multiply by each numerator factor, reducing gcd with denom. */
        seq_t num;

        num = (seq_t)(i + 4);
        v *= num;
        seq_t g = gcd(v, denom);
        if (g > 1) { v /= g; denom /= g; }

        num = (seq_t)(i + 3);
        v *= num;
        g = gcd(v, denom);
        if (g > 1) { v /= g; denom /= g; }

        num = (seq_t)(i + 2);
        v *= num;
        g = gcd(v, denom);
        if (g > 1) { v /= g; denom /= g; }

        num = (seq_t)(i + 2);        /* second (n+2) factor */
        v *= num;
        g = gcd(v, denom);
        if (g > 1) { v /= g; denom /= g; }

        /* Any remaining denominator factor must divide v exactly. */
        if (denom != 1) v /= denom;

        cache[i] = v;
    }

    cur_size = n + 1;
    return cache[n];
}
