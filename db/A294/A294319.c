/*
 * A294319  a(n) = Product_{k=0..n} (3*k + 2)!.
 * https://oeis.org/A294319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0..n} (3*k + 2)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorial.h"   /* factorial(idx_t n) returns seq_t */

static seq_t *cache = NULL;   /* cache[0..last] = a(0)...a(last) */
static idx_t cache_size = 0;  /* number of stored entries */

/* Return a(n) = Π_{k=0..n} (3*k + 2)! */
seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* increase cache to hold terms up to n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
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
