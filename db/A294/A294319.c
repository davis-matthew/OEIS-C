/*
 * A294319  a(n) = Product_{k=0..n} (3*k + 2)!.
 * https://oeis.org/A294319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (3*k + 2)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static seq_t *cache = NULL;   /* cache[0..last] = a(0)...a(last) */
static idx_t cache_size = 0;  /* number of stored entries */

/* Return a(n) = Π_{k=0..n} (3*k + 2)! */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* increase cache to hold terms up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) return (seq_t)0;          /* out of memory: return 0 */
        cache = tmp;

        /* start with the product of all terms already cached */
        seq_t prod = (cache_size == 0) ? (seq_t)1 : cache[cache_size - 1];

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t f = factorial(3 * i + 2);  /* (3*i+2)! */
            prod = prod * f;                 /* extend product */
            cache[i] = prod;
        }
        cache_size = new_size;
    }
    return cache[n];
}
