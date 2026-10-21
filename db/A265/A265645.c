/*
 * A265645  a(n) = n^2 * floor(n/2).
 * https://oeis.org/A265645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * floor(n/2)
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for type safety */
    seq_t nn = (seq_t)n;

    /* `nn*nn` computes n².  For seq_t up to 128 bits this is safe for n up to 1e4. */
    seq_t sq = nn * nn;

    /* floor(n/2) is simply the right‑shift by one bit.  Using shift is
       equivalent to integer division for unsigned types. */
    seq_t half = nn >> 1;

    /* Multiply the two integers. */
    return sq * half;
}
