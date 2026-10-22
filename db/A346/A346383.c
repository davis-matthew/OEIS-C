/*
 * A346383  Numbers that leave their digital root as the remainder when the
 *          product of their digits is divided by the sum of their digits.
 * https://oeis.org/A346383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2337, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, digit_sum, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = numbers x such that digital_root(x) == (product_of_digits(x) % sum_of_digits(x)) */
/* uses a brute‑force search with memoization */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* stored terms */
static size_t cache_cap = 0;         /* capacity of cache */
static size_t cache_len = 0;         /* number of terms stored */
static idx_t last_checked = 0;       /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safeguard for bad inputs */

    /* extend the cache until it contains at least n terms */
    while (cache_len < n) {
        ++last_checked;                  /* next candidate number */
        seq_t x = last_checked;           /* cast to seq_t */

        seq_t s = digit_sum(x);           /* sum of digits */
        if (s == 0) continue;             /* should not happen for positive x */

        seq_t p = digit_product(x);       /* product of digits */

        /* digital root of x (1..9 for positive x) */
        seq_t dr = x % 9;
        if (dr == 0) dr = 9;

        if (p % s == dr) {
            /* ensure capacity */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *new_cache = seq_alloc_new_array(new_cap);
                for (size_t i = 0; i < cache_len; ++i) {
                    new_cache[i] = cache[i];
                }
                if (cache) seq_free_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;        /* store the term */
        }
    }

    return cache[n - 1];                 /* zero‑based index */
}
