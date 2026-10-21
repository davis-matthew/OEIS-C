/*
 * A279662  a(n) = (2/3)^n*Gamma(n+3/4)*Gamma(n+1)*Gamma(n+2)/Gamma(3/4).
 * https://oeis.org/A279662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} k*(k+1)*(4*k-1)/6; a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* compute the k‑th factor as an integer */
        seq_t term = (seq_t)k * (k + 1) * (((seq_t)4 * k) - (seq_t)1);
        term /= (seq_t)6;          /* exact division */
        result *= term;
    }
    return result;
}
