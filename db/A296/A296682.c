/*
 * A296682  Number of n X 2 0..1 arrays with each 1 adjacent to 1, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n‑1) - a(n‑2) + 11*a(n‑3) - 2*a(n‑4) + 8*a(n‑5) -
 *            4*a(n‑6), for n ≥ 7 with initial terms: a(1)=2, a(2)=12,
 *            a(3)=43, a(4)=145, a(5)=524, a(6)=1888
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;          /* 1‑based: cache[1] == a(1) */
    static size_t cache_cap = 0;         /* allocated capacity (size_t) */
    static idx_t last_computed = 6;      /* highest n for which cache[n] is valid */

    /* initialise the first six terms once */
    if (cache == NULL) {
        /* allocate space for indices 0..6 (index 0 unused) */
        size_t init_cap = 8; /* 0..7 for safety */
        ensure_capacity(init_cap, &cache_cap, (void**)&cache, sizeof(seq_t));

        cache[1] = (seq_t)2;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)43;
        cache[4] = (seq_t)145;
        cache[5] = (seq_t)524;
        cache[6] = (seq_t)1888;
        last_computed = 6;
    }

    /* trivial case */
    if (n <= 6) return cache[n];

    /* ensure we have enough room to store up to n */
    if ((size_t)n + 2 > cache_cap) {
        size_t new_cap = (size_t)n + 2;
        ensure_capacity(new_cap, &cache_cap, (void**)&cache, sizeof(seq_t));
    }

    /* extend the cache up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 3 * cache[i - 1];
        sum += 11 * cache[i - 3];
        sum += 8  * cache[i - 5];

        seq_t sub = cache[i - 2];
        sub += 2 * cache[i - 4];
        sub += 4 * cache[i - 6];

        cache[i] = sum - sub;   /* sum >= sub for this sequence */
    }
    last_computed = n;

    return cache[n];
}
