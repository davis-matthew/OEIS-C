/*
 * A327750  Numbers without zero digits such that after adding the product of
 *          its digits to it, a number with the same product of digits is
 *          obtained.
 * https://oeis.org/A327750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 674, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A327750:
   a(n) = the n-th integer k that has no zero digits and
          satisfies product_of_digits(k) == product_of_digits(k + product_of_digits(k)).
   Numbers are generated in increasing order. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/append_candidate.h"

/* external linkage */
seq_t a(idx_t n)
{
    /* cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* capacity of the cache */
    static idx_t  count = 0; /* number of terms currently stored */
    static idx_t  last_checked = 0; /* last integer examined */

    if (n == 0) return 0; /* offset starts at 1, undefined for n==0 */

    /* If requested term is already computed, return it. */
    if (n <= count) {
        return cache[n - 1];
    }

    /* Generate terms until we have at least n terms */
    for (idx_t m = last_checked + 1; ; ++m)
    {
        seq_t k = (seq_t)m;                     /* candidate k */
        seq_t prod = digit_product(k);          /* product of digits of k */

        if (prod == 0) {                       /* k contains a zero digit */
            last_checked = m;
            continue;
        }

        seq_t k2 = k + prod;                    /* k + product(k) */
        seq_t prod2 = digit_product(k2);        /* product of digits of k2 */

        if (prod2 == prod) {                    /* property satisfied */
            /* store the new term in the cache */
            append_candidate(&cache, &count, &cap, k);
            if (count == n) {
                last_checked = m;
                return k;
            }
        }

        last_checked = m;
    }
}
