/*
 * A300659  Product of digits of n!.
 * https://oeis.org/A300659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of digits of n! ; a(n) = 0 for n >= 5
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as the product of all decimal digits of n!. 
   Since n! ends with at least one trailing zero for n ≥ 5, the product
   is 0 for all n ≥ 5.  For n < 5 the factorization of n! is tiny,
   so we compute it directly and apply the digit_product helper. */

#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* For n ≥ 5 the factorial has a trailing zero, so the digit product is 0. */
    if (n >= 5)
        return 0;

    /* Compute n! for small n and return the product of its digits. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    return digit_product(fact);
}
