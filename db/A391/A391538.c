/*
 * A391538  Guaranteed win if opponent chooses one of three baskets and the
 *          player chooses the coins with values from 1 to n (see Comments for
 *          details).
 * https://oeis.org/A391538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1, 2, 3, 5 for n = 1..4; for n > 4, a(n) =
 *            floor(n*(n-1)/6) + n
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;
    /* n > 4 */
    seq_t t = (seq_t)n * (seq_t)(n - 1) / (seq_t)6 + (seq_t)n;
    return t;
}
