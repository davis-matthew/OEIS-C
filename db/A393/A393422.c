/*
 * A393422  Irregular table T(n, k), n > 0, k = 1..2^(n-1), read by rows; row
 *          1 contains a single 0; for n >= 1, row n+1 contains the terms of
 *          row n plus n^2 followed by the terms of row n minus n^2.
 * https://oeis.org/A393422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf, base
 * Formula:   a(n) = Sum_{k=1}^{⌊log2(n)⌋} k^2 * (-1)^{b_k} where b_k is the
 *            (k-1)-th bit of n after the leading 1 in its binary expansion
 * Range:     i8 n <= 127, i16 n <= 8191, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 for the OEIS sequence */
    seq_t sum = 0;

    /* If n is 1 the loop below contributes nothing, so we can just return 0. */
    if (n <= 1)
        return 0;

    /* Determine the position of the most significant bit (zero‑based). */
    idx_t msb_pos = 0;          /* number of bits after the leading 1 */
    idx_t temp = n;
    while (temp > 1) {
        temp >>= 1;
        ++msb_pos;
    }

    /* For each bit below the leading one compute the contribution. */
    for (idx_t i = 0; i < msb_pos; ++i) {
        idx_t bit = (n >> i) & 1;
        seq_t sq = (seq_t)(i + 1) * (seq_t)(i + 1);
        if (bit == 0)
            sum += sq;
        else
            sum -= sq;
    }

    return sum;
}
