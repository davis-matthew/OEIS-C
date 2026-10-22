/*
 * A308726  The number of permutations of length n and tier at most 1, that
 *          is, the number of permutations of length n sortable by two passes
 *          through a stack where outputting the longest prefix matching the
 *          identity permutation is prioritized.
 * https://oeis.org/A308726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_compute, small_compute
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308726
   A(n) = number of permutations of length n sortable by two passes through a stack
   following a specific priority rule.
   Recurrence (conjectured D‑finite):
   3*n*(n-1)*(n+1)*a(n) -n*(n-1)*(67*n-101)*a(n-1)
   +2*(n-1)*(286*n*n-1112*n+1089)*a(n-2)
   +4*(-580*n*n*n+4200*n*n-10106*n+8049)*a(n-3)
   +24*(184*n*n*n-1784*n*n+5770*n-6221)*a(n-4)
   -96*(4*n-15)*(2*n-9)*(4*n-17)*a(n-5) = 0
   Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=24.
   This implementation caches computed terms and works for any seq_t size:
   • 8‑/16‑/32‑/64‑bit unsigned integers,
   • `unsigned __int128`,
   • arbitrary‑precision integer classes exported by the build (operator+, -, *, / defined). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_compute.h"
#include "../utils/small_compute.h"

#define INITIAL_CAP 16

/* Ensure the cache array can hold indices up to n. */
static void
ensure_cache(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if (needed <= *cap) return;

    size_t new_cap = needed * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;            /* allocation failure – abort */
    if (*cache) {
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*cache)[i];
        free(*cache);
    }
    *cache = new_arr;
    *cap   = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 4;     /* a(0) … a(4) are known */

    /* Initialize cache if needed. */
    if (!cache) {
        cache_cap = INITIAL_CAP;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – return 0 */
        cache[0] = 1; cache[1] = 1;
        cache[2] = 2; cache[3] = 6;
        cache[4] = 24;
    }

    if (n <= last_computed) return cache[n];

    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Use the small routine only for types that fit in an unsigned 64‑bit integer. */
        if (sizeof(seq_t) <= 8) {
            cache[i] = small_compute(i, cache);
        } else {
            cache[i] = big_compute(i, cache);
        }
    }
    last_computed = n;
    return cache[n];
}
