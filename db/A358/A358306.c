/*
 * A358306  Second row of array in A358304.
 * https://oeis.org/A358306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: for n even, a(n) = (7*(n/2)^2 + 13*(n/2)) / 2;
 *           for n odd,  a(n) = (7*((n-1)/2)^2 + 21*((n-1)/2) + 10) / 2
 * This implements the closed‑form for A358306.
 */
seq_t a(idx_t n)
{
    /* compute m = floor(n/2) using integer division */
    seq_t m = (seq_t)(n >> 1);  /* n/2 */

    if ((n & 1) == 0)  /* even n */
    {
        return ((seq_t)7 * m * m + (seq_t)13 * m) / (seq_t)2;
    }
    else               /* odd n */
    {
        return ((seq_t)7 * m * m + (seq_t)21 * m + (seq_t)10) / (seq_t)2;
    }
}
