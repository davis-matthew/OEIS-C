/*
 * A254597  Convert number n to the bases from 2 to 10. For any n, sequence
 *          lists the minimum base with the highest product of digits.
 * https://oeis.org/A254597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the base (2..10) that maximizes the product of the digits
   of n when n is expressed in that base.
   In case of a tie among several bases, returns the smallest base.
 */

#include <stdbool.h>   /* required for C11/C++17 compatibility */
#include "../utils/digit_product_base.h"

seq_t a(idx_t n)
{
    idx_t base;
    idx_t best_base = 2;    /* smallest possible base */
    seq_t best_prod = 0;    /* product for best_base */

    for (base = 2; base <= 10; ++base) {
        seq_t prod = digit_product_base(n, base);
        if (prod > best_prod ||
            (prod == best_prod && (seq_t)base < (seq_t)best_base)) {
            best_prod  = prod;
            best_base  = base;
        }
    }
    return (seq_t)best_base;
}
