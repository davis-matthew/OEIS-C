/*
 * A266491  a(n) = n*A130658(n).
 * https://oeis.org/A266491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3 - (-1)^((n-1)*n/2))/2
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If floor(n/2) is even the term equals n; if odd it equals 2*n */
    seq_t v = (seq_t)n;                   /* convert index to seq_t type */
    if (((n / 2) & 1) == 0)
        return v;                        /* floor(n/2) even */
    else
        return v + v;                    /* floor(n/2) odd, term = 2*n */
}
