/*
 * A260861  Base-11 representation of a(n) is the concatenation of the base-11
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = integer whose base‑11 representation is the concatenation
 *            of the base‑11 representations of 1,2,…,n,n-1,…,1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   digits_in_base11, pow11_of_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits_in_base11.h"
#include "../utils/pow11_of_len.h"

seq_t a(idx_t n) {
    seq_t res = 0;

    /* ascending part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits_in_base11(i);
        seq_t mult = pow11_of_len(d);
        res = res * mult + (seq_t)i;
    }

    /* descending part: n-1 .. 1 */
    for (idx_t i = n; i > 1; --i) {
        int d = digits_in_base11(i - 1);
        seq_t mult = pow11_of_len(d);
        res = res * mult + (seq_t)(i - 1);
    }

    return res;
}
