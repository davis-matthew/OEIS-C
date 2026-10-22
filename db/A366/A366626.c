/*
 * A366626  Group the natural numbers into blocks of size 2: [1,2], [3,4], ...
 *          and then reverse the order of the numbers within each block. Then
 *          group and reverse for each block size up to 4.
 * https://oeis.org/A366626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A366619(A113778(n))
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: apply reversal of 2‑, 3‑ and 4‑sized blocks
   to the natural numbers.  The value at position n in the
   final sequence is the original index that ends up there.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t pos = n;                     /* current position in the sequence */

    /* reverse blocks of size 4 */
    idx_t offset = (pos - 1) % 4;
    pos = pos + 3 - 2 * offset;

    /* reverse blocks of size 3 */
    offset = (pos - 1) % 3;
    pos = pos + 2 - 2 * offset;

    /* reverse blocks of size 2 */
    if (pos & 1)   /* odd index: comes from the next larger odd position */
        pos = pos + 1;
    else           /* even index: comes from the preceding odd position */
        pos = pos - 1;

    return (seq_t)pos;
}
