/*
 * A336682  a(n) is the number of iterations needed to reach a fixed point
 *          starting with n and repeatedly applying f(x) = x - (the product of
 *          the digits of x).
 * https://oeis.org/A336682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 287, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A336682
   a(n) = number of iterations needed to reach a fixed point
          starting with n and repeatedly applying f(x) = x - product_of_digits(x).
   Fixed points are exactly the integers that contain a digit 0,
   because then the product of digits is 0 and f(x)=x. */

#include <stdint.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;     /* current value in the iteration */
    seq_t cnt = 0;          /* number of iterations performed */

    while (1) {
        seq_t prod = digit_product(x);
        if (prod == 0)     /* x already a fixed point (contains a zero digit) */
            break;
        x = x - prod;      /* apply the transformation */
        cnt++;
    }
    return cnt;
}
