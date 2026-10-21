/*
 * A257083  Partial sums of A257088.
 * https://oeis.org/A257083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = * if n is even: 3*(n/2)^2 + 2*(n/2) + 1 * if n is odd :
 *            3*(n/2)^2 + 4*(n/2) + 2 * This implements the closed‑form
 *            expression from OEIS A257083
 * Range:     u8 n <= 17, u16 n <= 295, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n >> 1);   /* k = floor(n/2) */
    if ((n & 1) == 0) {          /* n even */
        return 3*k*k + 2*k + 1;
    } else {                     /* n odd */
        return 3*k*k + 4*k + 2;
    }
}
