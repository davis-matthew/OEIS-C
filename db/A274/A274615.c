/*
 * A274615  Third column (that is, the c=2 column) of array in A274528.
 * https://oeis.org/A274615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, write n = 16t + i with 0 ≤ i ≤ 15. Then
 *            a(n) = 16t + c[i] where c =
 *            {-1,4,5,2,0,8,3,6,7,12,13,10,11,9,17,14}
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* special case */

    /* constant offsets for the 16 positions of a full block of 16 */
    static const int c[16] = {
        -1, 4, 5, 2, 0, 8, 3, 6,
         7,12,13,10,11, 9,17,14
    };

    idx_t i = n % 16;                     /* position within the block (0‑15) */
    seq_t base = (seq_t)(n - i);          /* 16 * floor(n/16) */

    if (i == 0)                           /* use the (-1) adjustment only when i==0 */
        return base - (seq_t)1;
    else
        return base + (seq_t)c[i];
}
