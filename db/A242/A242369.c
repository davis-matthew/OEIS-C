/*
 * A242369  a(n) = P(n, 1, -2*n-1, 1-2*n)/(n+1), P the Jacobi polynomial.
 * https://oeis.org/A242369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{j=0}^{n-1} C(n,j) * C(n,j+1) * n^j (derived
 *            from a(n) = Σ_{j=0}^{n-1} binom(n,j)^2 * (n-j)/(j+1) * n^(j-1)
 *            for n>0, with a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t sum = 0;                /* accumulator for the sum */
    seq_t B    = 1;                /* B = C(n,0) initially */
    seq_t pow_n = 1;               /* n^j, starting with j = 0 */

    for (idx_t j = 0; j < n; ++j) {
        /* Compute C(n,j+1) from C(n,j) */
        seq_t B_next = B * ((seq_t)n - (seq_t)j) / ((seq_t)(j + 1));

        /* Term = C(n,j) * C(n,j+1) * n^j */
        seq_t term = B * B_next * pow_n;
        sum += term;

        /* Advance to next j */
        B = B_next;
        pow_n *= (seq_t)n;
    }

    /* Finally divide by n to obtain a(n) */
    return sum / (seq_t)n;
}
