/*
 * A306607  The bottom entry in the difference table of the binary digits of
 *          n.
 * https://oeis.org/A306607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = Sum_{k=0..msb} (-1)^(msb-k) * binomial(msb,k) * bit_k,
 *            where msb = floor(log2(n)) and bit_k is the k-th
 *            least‑significant binary digit of n. This equals the bottom
 *            entry of the difference table of the bits of n
 * Range:     i8 n <= 340, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_choose.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Find msb = floor(log2(n)) */
    idx_t msb = 0;
    while ((n >> msb) != 0) ++msb;
    msb--;                          /* msb is now the index of the highest set bit */

    seq_t sum = 0;
    for (idx_t k = 0; k <= msb; ++k) {
        if (((n >> k) & 1ULL) == 0) continue;   /* skip zero bits */
        seq_t bin = _choose(msb, k);
        if ((msb - k) & 1)          /* odd -> negative sign */
            sum -= bin;
        else                        /* even -> positive sign */
            sum += bin;
    }
    return sum;
}
