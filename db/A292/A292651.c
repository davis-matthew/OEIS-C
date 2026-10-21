/*
 * A292651  Rank of n*Pi when all the numbers j*e and k*Pi, for j>=1, k>=1,
 *          are jointly ranked.
 * https://oeis.org/A292651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n·π/e⌋, using the rational approximation π/e ≈
 *            1155727901/1000000000
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of π/e with 9 decimal digits. */
    static const seq_t NUM = (seq_t)1155727901ULL;  /* numerator  */
    static const seq_t DEN = (seq_t)1000000000ULL;  /* denominator */

    /* Compute floor(n * NUM / DEN). */
    seq_t prod = (seq_t)n * NUM;
    seq_t q    = prod / DEN;

    /* Return n + floor(n*π/e). */
    return (seq_t)n + q;
}
