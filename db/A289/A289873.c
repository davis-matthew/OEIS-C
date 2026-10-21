/*
 * A289873  Related to perfect Wichmann rulers: a(n) = ( n^2 - (mod(n, 6) -
 *          3)^2 ) / 3.
 * https://oeis.org/A289873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( n^2 - (mod(n, 6) - 3)^2 ) / 3
 * Range:     u8 n <= 16, u16 n <= 442, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n² */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* (mod(n,6)-3)²  (value is in the range -3..2, so the square fits in a small integer) */
    int64_t s = (int64_t)(n % 6) - 3;          /* s ∈ { -3, -2, -1, 0, 1, 2 } */
    seq_t s_sq = (seq_t)(s * s);

    /* numerator: n² - (mod(n,6)-3)² */
    seq_t num = n_sq - s_sq;

    /* denominator is 3; the division is exact by definition of the sequence */
    seq_t res = num / 3;

    return res;
}
