/*
 * A384599  Expansion of (1+3*x+2*x^2) / (1-4*x^2-2*x^3).
 * https://oeis.org/A384599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 917 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3, a(2)=6; for n≥3, a(n)=4*a(n-2)+2*a(n-3) */
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
    static size_t cap   = 0;
    static idx_t  max_computed = 2;   /* we will have a[0], a[1], a[2] pre‑filled */

    /* Initialise cache on first call */
    if (cache == NULL) {
        grow_seq(&cache, &cap, 3);   /* need indices 0,1,2 */
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 6;
    }

    /* Grow and compute up to requested n if necessary */
    if (n > max_computed) {
        grow_seq(&cache, &cap, n + 1);   /* ensure capacity for indices 0..n */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence valid for i ≥ 3; the loop starts at ≥3 */
            cache[i] = ((seq_t)4 * cache[i - 2]) + ((seq_t)2 * cache[i - 3]);
        }
        max_computed = n;
    }

    return cache[n];
}
