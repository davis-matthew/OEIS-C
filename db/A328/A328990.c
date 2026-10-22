/*
 * A328990  a(n) = (3*b(n) + b(n-1) + 1)/2, where b = A005409.
 * https://oeis.org/A328990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) for n>3 Initial values: a(1) =
 *            2, a(2) = 7, a(3) = 19
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;   /* 0‑based array: cache[0] = a(1) */
static size_t cap   = 0;      /* allocated capacity (in elements) */
static size_t len   = 0;      /* number of terms already computed */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; behaviour below offset is irrelevant */

    /* Allocate or grow cache to hold at least n terms (1‑based) */
    ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms up to n */
    for (size_t i = len; i < (size_t)n; ++i) {
        if (i == 0)          /* a(1) */
            cache[i] = (seq_t)2;
        else if (i == 1)     /* a(2) */
            cache[i] = (seq_t)7;
        else if (i == 2)     /* a(3) */
            cache[i] = (seq_t)19;
        else                 /* recurrence for i >= 3 (i is 0‑based index) */
            cache[i] = (seq_t)(3 * cache[i - 1] - cache[i - 2] - cache[i - 3]);
    }
    len = (size_t)n;           /* now we have at least n terms cached */

    return cache[(size_t)n - 1];
}
