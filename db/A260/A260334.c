/*
 * A260334  a(n) = (36*n^6 - 60*n^5 + 30*n^4 + 4*n^3 + 8*n^2 - 4*n + 1 -
 *          (-1)^n)/8.
 * https://oeis.org/A260334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (36*n^6 - 60*n^5 + 30*n^4 + 4*n^3 + 8*n^2 - 4*n + 1 -
 *            (-1)^n)/8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow in intermediate calculations. */
    typedef unsigned __int128 u128;

    u128 N  = n;          /* n as u128 */
    u128 n2 = N * N;      /* n^2 */
    u128 n3 = n2 * N;     /* n^3 */
    u128 n4 = n3 * N;     /* n^4 */
    u128 n5 = n4 * N;     /* n^5 */
    u128 n6 = n5 * N;     /* n^6 */

    /* Polynomial part. */
    u128 num = 36u * n6
             - 60u * n5
             + 30u * n4
             + 4u * n3
             + 8u * n2
             - 4u * N
             + 1u;

    /* Subtract (-1)^n:  +1 for even n, -1 for odd n. */
    u128 s = (n & 1) ? (u128)-1 : (u128)1;   /* -1 in unsigned arithmetic is 2^128-1 */
    num = num - s;                            /* subtract (-1)^n */

    /* Divide by 8. */
    num >>= 3;

    return (seq_t)num;
}
