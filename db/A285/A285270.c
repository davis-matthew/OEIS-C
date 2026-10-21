/*
 * A285270  a(n) = H_n(n), where H_n is the physicist's n-th Hermite
 *          polynomial.
 * https://oeis.org/A285270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = H_n(n), where H_n is the physicists' n-th Hermite
 *            polynomial
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    const seq_t two = (seq_t)2;
    const seq_t xn   = (seq_t)n;
    const seq_t factor_n = xn * two;          /* 2*n */

    seq_t h_prev = (seq_t)1;                  /* H_0(n) */
    seq_t h_curr = factor_n;                 /* H_1(n) = 2*n */

    for (idx_t k = 1; k < n; ++k) {
        const seq_t factor_k = (seq_t)k * two;     /* 2*k */
        const seq_t h_next = factor_n * h_curr - factor_k * h_prev;
        h_prev = h_curr;
        h_curr = h_next;
    }
    return h_curr;
}
