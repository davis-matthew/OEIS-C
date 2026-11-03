/*
 * A374662  a(n) = (1/2)*Product_{k=0..n} (F(k)+2), where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A374662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*Product_{k=0..n} (F(k)+2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

static seq_t *cache = NULL;      /* cached a(k) values */
static size_t cache_cap = 0;     /* current allocation size */
static idx_t next_to_compute = 0;/* next index that has not yet been computed */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while ((size_t)n >= new_cap)
        new_cap *= 2;

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Resize cache if needed. */
    ensure_cache(n);

    /* Compute any missing terms in order. */
    while (next_to_compute <= n) {
        if (next_to_compute == 0) {
            /* a(0) = 1 */
            cache[0] = (seq_t)1;
        } else {
            /* a(k) = a(k-1) * (F(k) + 2) */
            seq_t f = fib(next_to_compute);
            cache[next_to_compute] = cache[next_to_compute - 1] * (f + (seq_t)2);
        }
        ++next_to_compute;
    }

    return cache[n];
}
