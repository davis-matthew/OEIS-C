/*
 * A357937  a(n) is the least multiple of n that is not a Niven (or Harshad)
 *          number.
 * https://oeis.org/A357937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 107, u32 n <= 7559, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least multiple of n that is not a Niven number */
/* The function iterates over multiples of n until one fails to be divisible by its digit sum. */

#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    seq_t seq_n = (seq_t)n;          /* convert index to seq_t */
    idx_t m = 1;                     /* multiplier */

    for (;;) {
        seq_t product = seq_n * (seq_t)m;
        seq_t dsum   = digit_sum(product);

        /* if product is not divisible by its digit sum, we have found a */
        if (product % dsum != 0) {
            return product;
        }
        ++m;                          /* try next multiple */
    }
}
