/*
 * A386588  a(n) = 2^a(n-1) mod n, with a(1) = 1.
 * https://oeis.org/A386588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = 2^a(n‑1) mod n (n ≥ 2)
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 8609 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;        /* index 0 unused, cache[1] = 1 */
static idx_t   max_computed = 0;   /* highest index already computed */
static size_t  cache_cap   = 0;    /* allocated array size */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined: keep harmless */

    /* Ensure the cache is large enough. */
    if (cache_cap < (size_t)(n + 1)) {
        idx_t new_cap = n + 1;           /* indices 0 … n */
        seq_t *tmp;
#if defined(__cplusplus)
        tmp = mem_resize(seq_t, cache, new_cap);
#else
        tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first element if this is the first call. */
    if (max_computed < 1) {
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t exp = cache[i - 1];
        /* a(i) = 2^exp mod i.  exp ≤ i, so it fits in uint64_t. */
        cache[i] = powmod((seq_t)2, (uint64_t)exp, (seq_t)i);
    }
    max_computed = n;
    return cache[n];
}
