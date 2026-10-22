/*
 * A302945  a(n) = 4*(2*n+1)(n*a(n-1) + (-1)^(n-1)*((n-1)!)^2), with a(0) = 0,
 *          n > 0.
 * https://oeis.org/A302945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(2*n+1)*(n*a(n-1) + (-1)^(n-1)*((n-1)!)^2); a(0) = 0;
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (!n) return (seq_t)0;

    seq_t a_prev = 0;
    seq_t fact   = 1;            /* 0! = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* ((i-1)!)^2 */
        seq_t f_sq = fact * fact;

        /* n * a(n-1) */
        seq_t prod = (seq_t)i * a_prev;

        /* sign factor: +1 for even (i-1), -1 for odd (i-1) */
        seq_t term;
        if ((i - 1) & 1U)
            term = prod - f_sq;   /* safe: prod >= f_sq for all n */
        else
            term = prod + f_sq;

        /* a(i) = 4 * (2*i + 1) * term */
        a_prev = 4 * (((seq_t)(2 * i + 1))) * term;

        /* update factorial for next iteration: fact = (i)! */
        fact *= (seq_t)i;
    }

    return a_prev;
}
