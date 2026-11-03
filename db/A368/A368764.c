/*
 * A368764  a(n) = n! * (1 + Sum_{k=0..n} binomial(k+3,4) / k!).
 * https://oeis.org/A368764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + binomial(n+3,4); a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
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
    /* Static cache: a[i] contains a(i) for 0 <= i < cache_len. */
    static seq_t *cache = NULL;      /* array of values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t cache_len = 0;      /* number of values currently stored */

    /* Ensure the cache covers the requested index. */
    if (n >= cache_len) {
        /* Grow the array to hold indices up to n (inclusive). */
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

        /* Compute all missing terms from the last stored value up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;               /* a(0) = 1 */
            } else {
                /* Recurrence: a(i) = i * a(i-1) + C(i+3,4) */
                cache[i] = (seq_t)i * cache[i - 1] + binomial(i + 3, 4);
            }
        }
        cache_len = n + 1;      /* Update the number of stored values. */
    }

    return cache[n];
}
