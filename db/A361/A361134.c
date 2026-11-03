/*
 * A361134  a(1) = 1, a(2) = 2; for n >= 3, a(n) = (n-1)^3 - a(n-1) - a(n-2).
 * https://oeis.org/A361134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n) = (n-1)^3 - a(n-1) - a(n-2) for n>=3
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    if (n == 1) return 1;
    if (n == 2) return 2;

    /* Ensure base values are stored */
    if (cache_size < 2) {
        grow_seq(&cache, &cache_cap, 2);
        cache[0] = 1;
        cache[1] = 2;
        cache_size = 2;
    }

    /* Expand cache if necessary */
    if (n > (idx_t)cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)n);
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t cube = (seq_t)(i - 1);
        cube *= cube;
        cube *= (i - 1);
        cache[i - 1] = cube - cache[i - 2] - cache[i - 3];
    }
    cache_size = n;

    return cache[n - 1];
}
