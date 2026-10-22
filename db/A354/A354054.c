/*
 * A354054  a(n) = Product_{k=0..n} (k^6 + 1).
 * https://oeis.org/A354054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^6 + 1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   product_range
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/product_range.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)-1)  /* just in case of unsigned wrap-around */
        return (seq_t)1;
    return product_range((idx_t)0, n);
}
