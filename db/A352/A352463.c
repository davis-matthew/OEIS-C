/*
 * A352463  Numbers k with the property that the product of the digits of k
 *          starts k.
 * https://oeis.org/A352463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digit_len, digit_product, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A352463: numbers k whose decimal digits product is a
 * prefix of k.  Uses brute‑force search with memoisation. */
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n==0 (return 0). */
    if (n == 0) return (seq_t)0;

    /* static cache of found sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k = 1;            /* next integer to test */

    /* if we already have enough terms, return cached value. */
    if (cache_used >= n) return cache[n - 1];

    /* initialise cache on first call. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have the requested index. */
    for (idx_t k = next_k; ; ++k) {
        seq_t val = (seq_t)k;               /* current candidate */

        /* product of the decimal digits of val. */
        seq_t prod = digit_product(val);
        if (prod == 0) continue;            /* any zero digit → impossible */

        int len_k = digit_len(val);
        int len_p = digit_len(prod);
        if (len_p > len_k) continue;        /* product longer than k → fail */

        /* prefix of val that may equal prod. */
        seq_t prefix = val / pow10((idx_t)(len_k - len_p));
        if (prefix == prod) {
            /* store the value into the cache. */
            if (cache_used == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                cache = tmp;
            }
            cache[cache_used++] = val;

            if (cache_used >= n) {
                next_k = k + 1;              /* resume from next integer next time */
                return cache[n - 1];
            }
        }
    }
}
