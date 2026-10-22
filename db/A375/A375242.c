/*
 * A375242  Numbers with digital root in {2, 4, 5, 7, 8}.
 * https://oeis.org/A375242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*floor((n-1)/5) + {1,3,4,6,7}[((n-1)%5)] + 1
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with digital root in {2,4,5,7,8} (offset 1). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1-based.  Compute the zero‑based index. */
    idx_t idx = n - 1;                 /* safe for n >= 1 */
    idx_t block = idx / 5;             /* which 9‑number block */
    idx_t pos   = idx % 5;             /* position within the block */

    /* Offsets inside a block of numbers 1 + 9*k: 2,4,5,7,8 corresponding to
       offsets 1,3,4,6,7 added to the block start. */
    static const seq_t offsets[5] = {
        (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)6, (seq_t)7
    };

    /* Assemble the value: 9*block + 1 + offsets[pos] */
    return (seq_t)(block * 9 + 1 + offsets[pos]);
}
