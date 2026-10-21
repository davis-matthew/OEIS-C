/*
 * A244542  Palindromes n such that n +/- the product of digits of n are both
 *          palindromes.
 * https://oeis.org/A244542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_product, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerate positive integers in increasing order, but only
   keep those that are palindromes in base 10 and for which both
   n ± product_of_digits(n) are also palindromes in base 10.
   The results are memoized in a static array so that calls for
   arbitrary n are efficient. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper utilities from the utils library */
#include "../utils/is_palindrome.h"
#include "../utils/digit_product.h"

/* Computation of A244542 */
seq_t a(idx_t n)
{
    /* Static cache of found values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Last integer examined; starts at 0 so that the first test
       begins with 1 automatically. */
    static idx_t last_tested = 0;

    if (n == 0)   /* undefined, but avoid deference of cache[-1] */
        return 0;

    /* Grow the cache until it contains at least n values */
    while (cache_len < n) {
        ++last_tested;
        seq_t cur = (seq_t)last_tested;

        /* Check if cur is a palindrome */
        if (!is_palindrome(cur, 10))
            continue;

        /* Compute product of digits */
        seq_t prod = digit_product(cur);

        /* Check the +/- conditions */
        seq_t plus  = cur + prod;
        seq_t minus = cur - prod;
        if (is_palindrome(plus, 10) && is_palindrome(minus, 10)) {
            /* Ensure there is space in the cache */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure; abort since we cannot recover */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
