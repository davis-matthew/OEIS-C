/*
 * A392172  Number of regions formed when n points are placed in general
 *          position on each edge of a square and a chord is drawn from each
 *          point to the 3*n points on the other three sides.
 * https://oeis.org/A392172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 7*n + 80*binom(n,2) + 252*binom(n,3) + 204*binom(n,4)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 150, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficients are computed with exact integer arithmetic. */
seq_t a(idx_t n)
{
    seq_t s  = (seq_t)n;
    seq_t b2 = s * (s - 1) / 2;          /* binom(n,2) */
    seq_t b3 = b2 * (s - 2) / 3;          /* binom(n,3) */
    seq_t b4 = b3 * (s - 3) / 4;          /* binom(n,4) */

    return (seq_t)1
           + (seq_t)7 * s
           + (seq_t)80 * b2
           + (seq_t)252 * b3
           + (seq_t)204 * b4;
}
