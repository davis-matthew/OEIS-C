/*
 * A306754  The bottom entry in the difference table of the positions of the
 *          ones in the binary representation of n.
 * https://oeis.org/A306754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = bottom entry of the difference table of the positions of
 *            the ones in the binary representation of n
 * Range:     i8 n <= 3998, i16 n <= 9470, i32 n <= 9470, i64 n <= 9470,
 *            i128 n <= 9470, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Special case: n = 0 has no 1‑bits.  */
    if (n == 0)
        return 0;

    /* The largest possible number of 1‑bits in a 64‑bit integer is 64.
       We keep an array of that size to avoid dynamic allocation. */
    seq_t pos[65];
    size_t h = 0;          /* number of 1‑bits found so far */
    idx_t temp = n;
    idx_t bitpos = 0;      /* current bit position, starting from 0 */

    /* Gather the positions of all set bits. */
    while (temp != 0) {
        if (temp & (idx_t)1)
            pos[h++] = (seq_t)bitpos;
        temp >>= 1;
        ++bitpos;
    }

    /* If there is only one 1‑bit, the bottom entry is that position. */
    if (h == 1)
        return pos[0];

    /* Compute successive finite differences until one value remains. */
    size_t len = h;
    seq_t *arr = pos;      /* pointer to the array we will modify in place */

    while (len > 1) {
        for (size_t i = 0; i < len - 1; ++i)
            arr[i] = arr[i + 1] - arr[i];
        --len;
    }

    return arr[0];
}
