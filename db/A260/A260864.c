/*
 * A260864  Base-14 representation of a(n) is the concatenation of the base-14
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the integer whose base‑14 representation is the
 *            concatenation of the base‑14 representations of 1,2,…,n,n−1,…,1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   append_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for the constexpr of 14 if needed */
#include "../utils/append_number.h"

seq_t a(idx_t n)
{
    seq_t val = 0;

    if (n == 0) {
        /* a(0) = 0 by definition */
        return 0;
    }

    /* Forward part: 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        append_number(k, &val);
    }

    /* Backward part: n‑1 .. 1 */
    if (n > 1) {
        for (idx_t k = n - 1; ; --k) {
            append_number(k, &val);
            if (k == 1) break;
        }
    }

    return val;
}
