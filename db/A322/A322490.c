/*
 * A322490  Numbers k such that k^k ends with 7.
 * https://oeis.org/A322490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*n - 7 for odd n, 10*n - 3 for even n
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;             /* convert index to the element type */
    seq_t prod = ns * 10;            /* 10 * n */
    if (ns & 1)                     /* odd n */
        return prod - (seq_t)7;
    else                            /* even n */
        return prod - (seq_t)3;
}
