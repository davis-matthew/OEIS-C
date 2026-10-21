/*
 * A271076  Number of ordered ways to write n as u^5 + v^4 + x^3 + 2*y^3 +
 *          3*z^3, where u, v , x, y and z are nonnegative integers with v >
 *          0.
 * https://oeis.org/A271076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered solutions to n = u^5 + v^4 + x^3 +
 *            2*y^3 + 3*z^3 with non‑negative integers u, v, x, y, z and v >
 *            0. The function caches results up to the largest n requested
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Recursive structure: static cache */
static seq_t *counts = NULL;
static idx_t   max_computed = 0;

/* Helper: compute integer powers using 128‑bit arithmetic to avoid overflow. */
static inline __int128 pow5(idx_t u) { __int128 t = u; return t*t*t*t*t; }
static inline __int128 pow4(idx_t v) { __int128 t = v; return t*t*t*t; }
static inline __int128 pow3(idx_t x) { __int128 t = x; return t*t*t; }

static void recompute_counts(idx_t max_n)
{
    /* Allocate zeroed array of seq_t values. */
    seq_t *new_counts = (seq_t *)calloc((size_t)max_n + 1, sizeof(seq_t));
    if (!new_counts) abort();

    /* Pre‑compute maximum values for each variable. */
    idx_t max_u = 0, max_v = 0, max_x = 0, max_y = 0, max_z = 0;

    for (idx_t u = 0;; ++u) {
        __int128 val = pow5(u);
        if (val > max_n) break;
        max_u = u;
    }
    for (idx_t v = 1;; ++v) {          /* v > 0 */
        __int128 val = pow4(v);
        if (val > max_n) break;
        max_v = v;
    }
    for (idx_t x = 0;; ++x) {
        __int128 val = pow3(x);
        if (val > max_n) break;
        max_x = x;
    }
    for (idx_t y = 0;; ++y) {
        __int128 val = 2 * pow3(y);
        if (val > max_n) break;
        max_y = y;
    }
    for (idx_t z = 0;; ++z) {
        __int128 val = 3 * pow3(z);
        if (val > max_n) break;
        max_z = z;
    }

    /* Enumerate all admissible 5‑tuples and update the count. */
    for (idx_t u = 0; u <= max_u; ++u) {
        __int128 u5 = pow5(u);
        for (idx_t v = 1; v <= max_v; ++v) {
            __int128 v4 = pow4(v);
            __int128 uv = u5 + v4;
            if (uv > max_n) break;
            for (idx_t x = 0; x <= max_x; ++x) {
                __int128 x3 = pow3(x);
                __int128 uvx = uv + x3;
                if (uvx > max_n) break;
                for (idx_t y = 0; y <= max_y; ++y) {
                    __int128 y3 = pow3(y);
                    __int128 uvxy = uvx + 2 * y3;
                    if (uvxy > max_n) break;
                    for (idx_t z = 0; z <= max_z; ++z) {
                        __int128 z3 = pow3(z);
                        __int128 val = uvxy + 3 * z3;
                        if (val > max_n) break;
                        /* Increment count at index (idx_t)val. */
                        new_counts[(idx_t)val] += (seq_t)1;
                    }
                }
            }
        }
    }

    /* Replace old cache. */
    if (counts) free(counts);
    counts = new_counts;
    max_computed = max_n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n > max_computed) recompute_counts(n);
    return counts[(idx_t)n];
}
