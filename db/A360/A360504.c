/*
 * A360504  Concatenate the ternary strings for 1,2,...,n-1, n, n-1, ..., 2,1.
 * https://oeis.org/A360504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the ternary representations of
 *            1,2,…,n-1,n,n-1,…,2,1, with the resulting decimal value
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   append_ternary_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/append_ternary_digits.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Forward part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        append_ternary_digits(&res, i);
    }

    /* Reverse part: n-1 .. 1 */
    for (idx_t i = n - 1; i > 0; --i) {
        append_ternary_digits(&res, i);
    }

    return res;
}
