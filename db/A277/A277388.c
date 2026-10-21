/*
 * A277388  Number of nonnegative solutions of a certain system of linear
 *          Diophantine equations depending on an odd parameter.
 * https://oeis.org/A277388
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(-1+n)*(3-2*n+n^2-n^3+2*n^4)/18
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* values below the offset are irrelevant */

    seq_t nn = n;                /* n as seq_t */

    /* Compute 2*n^5 - 3*n^4 + 2*n^3 - 3*n^2 + 5*n - 3 using Horner's rule */
    seq_t t = (seq_t)2 * nn - (seq_t)3;    /* 2*n - 3 */
    t = t * nn + (seq_t)2;                /* (2*n - 3)*n + 2 */
    t = t * nn - (seq_t)3;                /* ((2*n - 3)*n + 2)*n - 3 */
    t = t * nn + (seq_t)5;                /* (((2*n - 3)*n + 2)*n - 3)*n + 5 */
    t = t * nn - (seq_t)3;                /* ((((2*n - 3)*n + 2)*n - 3)*n + 5)*n - 3 */

    seq_t S = t * nn;                     /* multiply by n to get the full 6th‑degree polynomial */

    return S / (seq_t)18;                 /* divide by 18 to obtain a(n) */
}
