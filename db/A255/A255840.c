/*
 * A255840  a(n) = (4*n^2 - 4*n + 1 - (-1)^n)/2.
 * https://oeis.org/A255840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2 - 4*n + 1 - (-1)^n)/2
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 0 , a(1) = 1 ,   etc.    */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t term   = (seq_t)4 * n_seq * n_seq - (seq_t)4 * n_seq + (seq_t)1;

    /* (-1)^n is  1  for even n, -1 for odd n.
       Therefore we subtract 1 for even, add 1 for odd. */
    if ((n & 1) == 0)
        term -= (seq_t)1;
    else
        term += (seq_t)1;

    return term / (seq_t)2;
}
