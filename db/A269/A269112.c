/*
 * A269112  a(n) = (3*(n-1)*n + (-1)^((n-1)*n/2) + 5)/2.
 * https://oeis.org/A269112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(n-1)*n + (-1)^((n-1)*n/2) + 5)/2
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t nm1   = n - 1;          /* n-1 */
    idx_t prod  = nm1 * n;        /* (n-1)*n  */
    idx_t parity = (prod >> 1) & 1; /* parity of ((n-1)*n/2) */

    idx_t term = 3 * prod + 5;    /* 3*(n-1)*n + 5 */

    if (parity == 0)              /* (-1)^k = +1 when parity even */
        term += 1;
    else                           /* (-1)^k = -1 when parity odd  */
        term -= 1;

    return (seq_t)(term / 2);     /* final division by 2 */
}
