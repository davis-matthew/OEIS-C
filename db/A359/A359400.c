/*
 * A359400  Sum of positions of zeros in the reversed binary expansion of n,
 *          where positions in a sequence are read starting with 1 from the
 *          left.
 * https://oeis.org/A359400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of positions of zeros in the reversed binary
 *            representation of n (positions counted from 1 on the left, i.e.
 *            from the least‑significant bit). a(0) = 1
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0)
        return 1;

    idx_t pos = 1;            /* Current bit position (starting at 1)   */
    idx_t tmp = n;            /* Working copy of n for bit extraction  */
    seq_t sum = 0;            /* Accumulated sum of positions of zeros*/

    while (tmp)
    {
        if (!(tmp & 1))               /* Bit is 0? */
            sum += (seq_t)pos;        /* Add its 1‑based position    */
        pos++;
        tmp >>= 1;                    /* Move to next higher bit      */
    }

    return sum;
}
