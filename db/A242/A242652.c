/*
 * A242652  Imaginary part of Product_{k=0..n} (i-k), where i=sqrt(-1).
 * https://oeis.org/A242652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the imaginary part of the product ∏_{k=0}^{n} (i - k).  */
/* The product is P(0) = i and P(n) = P(n-1)*(i - n).  */
/* Let P(n) = u_n + i*v_n; then */
/*   u_n = -n*u_{n-1} - v_{n-1} */
/*   v_n =  u_{n-1} - n*v_{n-1} */
/* with u_0 = 0, v_0 = 1.  */
/* The returned value a(n) is v_n. */
seq_t a(idx_t n)
{
    seq_t u = 0;      /* real part of P(k) */
    seq_t v = 1;      /* imaginary part of P(k) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t wk   = (seq_t)k;
        seq_t next_u = - (wk * u) - v;
        seq_t next_v = u - (wk * v);
        u = next_u;
        v = next_v;
    }
    return v; /* a(n) = Im(P(n)) */
}
