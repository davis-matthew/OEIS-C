/*
 * A260023  a(1)=77; thereafter form the product of the digits of the previous
 *          term.
 * https://oeis.org/A260023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=77; a(n)=digit_product(a(n-1)) for n>1 */
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined, but return 0 to avoid UB */
        return 0;
    }

    seq_t val = 77; /* a(1) */
    if (n == 1) {
        return val;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (val == 8) {
            /* All subsequent terms are 8 */
            return 8;
        }
        val = digit_product(val);
    }
    return val;
}
