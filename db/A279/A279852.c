/*
 * A279852  Number of n X 3 0..2 arrays with no element unequal to a strict
 *          majority of its horizontal and vertical neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 9*a(n-4) - 4*a(n-5) - 4*a(n-6) (n >
 *            9) Initial values: a(1)=2, a(2)=10, a(3)=49, a(4)=168, a(5)=557,
 *            a(6)=1758, a(7)=5441, a(8)=16500, a(9)=49253
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 209 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* compute a single term using the recurrence, using 128‑bit intermediates
   when seq_t is 64‑bit or less */
static inline seq_t recurrence_term(seq_t a1, seq_t a2, seq_t a4, seq_t a5, seq_t a6)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 t1 = (unsigned __int128)a1 * 4;
        unsigned __int128 t2 = (unsigned __int128)a2 * 2;
        unsigned __int128 t3 = (unsigned __int128)a4 * 9;
        unsigned __int128 t4 = (unsigned __int128)a5 * 4;
        unsigned __int128 t5 = (unsigned __int128)a6 * 4;
        unsigned __int128 res = t1 - t2 - t3 - t4 - t5;
        return (seq_t)res;
    } else {
        return a1 * 4 - a2 * 2 - a4 * 9 - a5 * 4 - a6 * 4;
    }
}

/* Base values for n = 1..9 (index 0 unused) */
static const seq_t base_vals[10] = {
    (seq_t)0,
    (seq_t)2,
    (seq_t)10,
    (seq_t)49,
    (seq_t)168,
    (seq_t)557,
    (seq_t)1758,
    (seq_t)5441,
    (seq_t)16500,
    (seq_t)49253
};

static seq_t *cache = NULL;        /* array of computed terms (1‑based) */
static idx_t cache_cap = 0;        /* current allocated capacity (number of slots) */
static idx_t cache_size = 0;       /* largest n for which cache[n] is valid */

/* grow the cache to accommodate at least "need" elements (1‑based) */
static void ensure_capacity(idx_t need)
{
    if (need + 1 <= cache_cap) return;

    size_t new_cap = (size_t)(need + 1) * 2 + 16;
    seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
    cache = new_arr;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined below offset */
    ensure_capacity(n);

    /* if already computed, return it */
    if (n <= cache_size) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 9)
            cache[i] = base_vals[i];
        else
            cache[i] = recurrence_term(cache[i-1], cache[i-2],
                                       cache[i-4], cache[i-5], cache[i-6]);
    }
    cache_size = n;
    return cache[n];
}
