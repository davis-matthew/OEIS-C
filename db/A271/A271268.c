/*
 * A271268  Concatenate sum of digits of previous term and product of digits
 *          of previous term, starting with 8.
 * https://oeis.org/A271268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concat( sum_of_digits(a(n-1)), prod_of_digits(a(n-1)) )
 *            with a(0) = 8 and concat(x,y) = x * 10^digits(y) + y The
 *            recurrence is evaluated lazily and cached
 * Range:     u8 n <= 1, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   digit_len, digit_product, digit_sum, ensure_cache, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/ensure_cache.h"

/* Return the n-th term (n >= 0).  The function keeps a static cache
   of previously computed terms so that calls with arbitrary n do not
   recompute again.  It works for any build where seq_t is an
   unsigned integer type or an arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* Static cache data */
    static seq_t *cache = NULL;       /* array of computed terms */
    static size_t cap = 0;            /* allocated size of cache array */
    static size_t cached = 0;         /* number of terms that have been computed */

    /* Ensure the cache is large enough for indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first term once. */
    if (cached == 0) {
        cache[0] = (seq_t)8;
        cached = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* Sum and product of decimal digits of the previous term. */
        seq_t sum = digit_sum(prev);
        seq_t prod = digit_product(prev);

        /* Number of decimal digits of the product. */
        int prod_len = digit_len(prod);

        /* Concatenate sum and product: sum * 10^prod_len + prod. */
        seq_t pow10v = pow10((idx_t)prod_len);
        cache[i] = sum * pow10v + prod;
    }

    cached = n + 1;
    return cache[n];
}
