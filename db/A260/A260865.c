/*
 * A260865  Base-15 representation of a(n) is the concatenation of the base-15
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of base‑15 representations of
 *            1,2,…,n,n‑1,…,1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   digits15, pow15_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds the value by repeatedly appending digits in base 15. */

#include <stdint.h>
#include "../utils/digits15.h"
#include "../utils/pow15_int.h"

/* OEIS sequence A260865. */
seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Append numbers 1 .. n. */
    for (idx_t i = 1; i <= n; ++i) {
        int d = digits15(i);
        seq_t mult = pow15_int(d);
        res = res * mult + (seq_t)i;
    }

    /* Append numbers n‑1 .. 1. */
    if (n > 0) {
        for (idx_t i = n - 1; i > 0; --i) {
            int d = digits15(i);
            seq_t mult = pow15_int(d);
            res = res * mult + (seq_t)i;
        }
    }

    return res;
}
