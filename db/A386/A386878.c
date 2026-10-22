/*
 * A386878  Number of runs of 1's of length <= 3 over all binary strings of
 *          length n.
 * https://oeis.org/A386878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 7*(n + 1)) * 2^(n-5) for n >= 5; a(0) = 0, a(1) = 1,
 *            a(2) = 3, a(3) = 8, a(4) = 19
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2_ex
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */
#include "../utils/pow2_ex.h"

seq_t a(idx_t n) {
    /* first five terms, exact */
    static const seq_t first[5] = {0, 1, 3, 8, 19};

    if (n < 5)
        return first[n];

    /* n >= 5: use formula */
    seq_t coeff = (seq_t)3 + (seq_t)7 * ((seq_t)n + 1);
    seq_t p = pow2_ex(n - 5);
    return coeff * p;
}
