/*
 * A253725  Integer squares c^2 that are equal to the sums of a number M(n) of
 *          consecutive cubed integers b^3 + (b+1)^3 + ... + (b+M-1)^3 = c^2,
 *          starting at b(n) (A002593) for M(n) being twice a squared integer
 *          (A001105).
 * https://oeis.org/A253725
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 * (4*n^4 - 1))^2
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 19, u128 n <= 463,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 254016, a(3) = 76055841, ... */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t nt  = (seq_t)n;        /* n */
    seq_t n2  = nt * nt;         /* n^2 */
    seq_t n3  = n2 * nt;         /* n^3 */
    seq_t n4  = n2 * n2;         /* n^4 */
    seq_t inner = 4 * n4 - 1;    /* 4*n^4 - 1 */
    seq_t prod  = n3 * inner;    /* n^3 * (4*n^4 - 1) */
    return prod * prod;          /* square */
}
