/*
 * A320039  Write n in binary, then modify each run of 0's and each run of 1's
 *          by appending a 1. a(n) is the decimal equivalent of the result.
 * https://oeis.org/A320039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal value of the binary string obtained from the
 *            binary representation of n by appending an extra '1' after each
 *            run of identical bits. Example: n = 6 (110₂) → 11101₂ = 29
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   msb_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/msb_index.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* defined only for n >= 1, but guard */

    seq_t result = 0;
    idx_t msb = msb_index(n);    /* position of highest 1‑bit */

    /* The top bit is always 1 */
    idx_t prev = (n >> msb) & 1;

    /* Process bits from msb‑1 down to 0 */
    for (idx_t i = msb; i > 0; --i) {
        /* Output the current run bit */
        result = (result << 1) | (seq_t)prev;

        /* Next bit in n */
        idx_t curr = (n >> (i - 1)) & 1;

        if (curr != prev) {
            /* Run ended – append an extra '1' */
            result = (result << 1) | (seq_t)1;
            prev = curr;
        }
    }

    /* Handle the final run: output the last bit and the appended 1 */
    result = (result << 1) | (seq_t)prev;  /* last run bit */
    result = (result << 1) | (seq_t)1;     /* appended 1 */

    return result;
}
