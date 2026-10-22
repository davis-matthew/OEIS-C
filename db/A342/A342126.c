/*
 * A342126  The binary expansion of a(n) corresponds to that of n where all
 *          the 1's have been replaced by 0's except in the first run of 1's.
 * https://oeis.org/A342126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n - A087734(n)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent description: keep the first run of 1's in the binary
   representation of n, set all remaining bits to zero. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the position of the most significant bit (0‑based). */
    idx_t msb_pos = 0;
    seq_t tmp = (seq_t)n;
    while (tmp > 1) {
        tmp >>= 1;
        msb_pos++;
    }

    /* Build the result by copying the leading run of 1’s. */
    seq_t result = 0;
    for (idx_t i = msb_pos; ; --i) {
        if (((seq_t)n >> i) & 1) {
            result |= ((seq_t)1 << i);
            if (i == 0) break;   /* finished the loop */
        } else {
            break;              /* end of the first run of 1’s */
        }
    }
    return result;
}
