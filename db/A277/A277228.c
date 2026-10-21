/*
 * A277228  Convolution of the even-indexed triangular numbers (A014105) and
 *          the squares (A000290).
 * https://oeis.org/A277228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,3)*(4*n^2 + 5*n + 4)/10
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 139, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A277228: Convolution of the even-indexed triangular numbers and the squares. */
seq_t a(idx_t n)
{
    /* For n = 0 or 1 the binomial coefficient is zero. */
    if (n < 2)
        return 0;

    seq_t nn   = (seq_t)n;
    seq_t n1   = nn + (seq_t)1;
    seq_t binom = n1 * nn * (nn - (seq_t)1) / (seq_t)6;
    seq_t term  = (seq_t)4 * nn * nn + (seq_t)5 * nn + (seq_t)4;

    return binom * term / (seq_t)10;
}
