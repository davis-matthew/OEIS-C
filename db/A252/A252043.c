/*
 * A252043  a(n) is the concatenation of first n terms of A033307.
 * https://oeis.org/A252043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(C*10^n) where C = 0.123456789101112...
 *            (Champernowne constant)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   append_int_to_res
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/append_int_to_res.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined for n >= 1; return 0 for safety. */
        return (seq_t)0;
    }

    idx_t remaining = n;
    seq_t result = (seq_t)0;

    for (idx_t k = 1; remaining > 0; ++k) {
        append_int_to_res(k, &result, &remaining);
    }

    return result;
}
