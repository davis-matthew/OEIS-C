/*
 * A276688  a(n) = Im([n]_{1+i}!), where [n]_q! is the q-factorial, i =
 *          sqrt(-1).
 * https://oeis.org/A276688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im([n]_{1+i}!) where [n]_{q} = ∏_{k=1}^n (1 - q^k)/(1 -
 *            q) with q = 1 + i. This implementation iteratively multiplies
 *            complex numbers, keeping only the real and imaginary parts as
 *            arbitrary‑precision integers (seq_t)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* At n = 0, the product is 1 and its imaginary part is 0. */
    seq_t re = (seq_t)1;   /* real part of the partial product */
    seq_t im = (seq_t)0;   /* imaginary part of the partial product */

    seq_t qt = (seq_t)1;   /* real part of q^k  */
    seq_t qu = (seq_t)0;   /* imaginary part of q^k */

    /* Iterate over k = 1 … n, updating q^k and multiplying in [k]_q. */
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Compute q^{k} = q^{k-1} * (1 + i) */
        seq_t t_next = qt - qu;          /* real part: t - u */
        seq_t u_next = qt + qu;          /* imag part: t + u */
        qt = t_next;
        qu = u_next;

        /* [k]_{1+i} = (q^k - 1) / i  -->
           real part  =  Im(q^k - 1)  = qu
           imag part  = -Re(q^k - 1) = -(qt - 1) */
        seq_t ak = qu;                 /* real part of [k]_q */
        seq_t bk = -(qt - (seq_t)1);   /* imag part of [k]_q */

        /* Multiply the running product by [k]_q */
        seq_t new_re = re * ak - im * bk;
        seq_t new_im = re * bk + im * ak;

        re = new_re;
        im = new_im;
    }

    return im;   /* Imaginary part of [n]_{1+i}! */
}
