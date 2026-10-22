/*
 * A366701  a(n) is the nearest integer to n*log_2(3/2).
 * https://oeis.org/A366701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*log_2(3/2)+0.5)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Approximate log2(3/2) by the rational 584962500721 / 1000000000000 */
    const seq_t NUM      = 584962500721ULL;
    const seq_t DEN      = 1000000000000ULL;
    const seq_t HALF_DEN = 500000000000ULL;

    return ((seq_t)n * NUM + HALF_DEN) / DEN;
}
