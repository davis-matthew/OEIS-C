/*
 * A367692  G.f. A(x) satisfies A(x) = 1 / ((1 - x) * (1 - x * A(x^4))).
 * https://oeis.org/A367692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..⌊(n-1)/4⌋} a(k) * a(n-1-4*k) (offset 0)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 53, u64 n <= 106,
 *            u128 n <= 212, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Allocate or grow the cache so that it can hold indices 0..n. */
    if (n + 1 > cache_size) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        /* New space may contain garbage – zero‑initialize it. */
        if (new_cap > cache_size)
            for (idx_t i = cache_size; i < new_cap; ++i)
                tmp[i] = 0;
        a_cache   = tmp;
        cache_size = new_cap;
    }

    /* Compute only the missing entries. */
    for (idx_t cur = 0; cur <= n; ++cur) {
        if (a_cache[cur] != 0)          /* already computed */
            continue;

        if (cur == 0) {
            a_cache[cur] = 1;
            continue;
        }

        seq_t sum = 0;
        idx_t lim = (cur - 1) / 4;
        for (idx_t k = 0; k <= lim; ++k) {
            idx_t j = cur - 1 - 4 * k;
            sum += a_cache[k] * a_cache[j];
        }
        a_cache[cur] = 1 + sum;
    }

    return a_cache[n];
}
