/*
 * A245017  Numbers k such that (product of digits of k) + 1 and (product of
 *          digits of k)^2 + 1 are both prime.
 * https://oeis.org/A245017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 565, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A245017: the n-th integer k such that
   (product_of_digits(k) + 1) is prime and (product_of_digits(k)^2 + 1)
   is also prime.  It memoises results so that calls arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static memoisation of found terms */
    static seq_t *terms = NULL;       /* array of found terms */
    static size_t cap   = 0;          /* capacity of terms array */
    static size_t len   = 0;          /* number of terms stored */
    static idx_t  checked = 0;        /* largest k examined so far */

    if (n == 0)                      /* offset is 1; undefined for n<1 */
        return (seq_t)0;

    /* If we already have the required term, return it immediately */
    if (n <= len)
        return terms[n - 1];

    /* Expand the search until we have at least n terms */
    while (len < n)
    {
        ++checked;
        seq_t k = (seq_t)checked;      /* current candidate */

        /* product of digits of k */
        seq_t p = digit_product(k);

        /* (p + 1) must be prime */
        seq_t p1 = p + (seq_t)1;
        if (!is_prime_big(p1))
            continue;

        /* (p^2 + 1) must be prime */
        seq_t p2 = p * p;
        seq_t p2plus = p2 + (seq_t)1;
        if (!is_prime_big(p2plus))
            continue;

        /* k satisfies the criterion – record it */
        append_candidate(&terms, &len, &cap, k);
    }

    return terms[n - 1];
}
