/*
 * A284375  Numbers whose product of digits is a power of 0.
 * https://oeis.org/A284375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest nonnegative integer m such that
 *            product_of_digits(m) ∈ {0, 1} (with 0^0 considered 1); *
 *            Equivalent to: all decimal numbers whose digits contain at least
 *            one zero or consist only of digits 1
 * Range:     u8 n <= 47, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_product, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/seq_realloc.h"

/* a – returns the n‑th term (1‑based) of OEIS sequence A284375.  */
seq_t a(idx_t n)
{
    /* static cache of terms we have already generated */
    static seq_t *cache      = NULL;   /* pointer to array of seq_t terms */
    static size_t cache_size = 0;      /* number of terms stored */
    static size_t cache_cap  = 0;      /* current capacity of the array */
    static uint64_t next_val  = 0;     /* next integer candidate to test */

    if (n == 0) /* undefined, but we return 0 for safety */
        return (seq_t)0;

    /* grow cache to contain at least n terms */
    if (cache_size < (size_t)n) {
        /* increase capacity if needed */
        if (cache_cap < (size_t)n) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort(); /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* generate terms until we have n of them */
        while (cache_size < (size_t)n) {
            /* find next integer whose digit product is 0 or 1 */
            while (1) {
                seq_t val   = (seq_t)next_val;
                seq_t prod  = digit_product(val);
                if (prod == (seq_t)0 || prod == (seq_t)1)
                    break;
                ++next_val;
            }
            /* store the found value */
            cache[cache_size] = (seq_t)next_val;
            ++cache_size;
            ++next_val;
        }
    }

    /* return the requested term */
    return cache[n - 1];
}
