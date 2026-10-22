/*
 * A373730  Reduced Collatz function R applied to the numbers 6n+1: a(n) =
 *          R(6n+1), where R(k) = (3k+1)/2^r, with r as large as possible.
 * https://oeis.org/A373730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = R(6n+1) where R(k) = (3k+1)/2^r with r maximal
 * Range:     u8 n <= 13, u16 n <= 4733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = 6n + 1 */
    seq_t k = (seq_t)n * 6 + 1;

    /* val = 3k + 1 */
    seq_t val = k * 3 + 1;

    /* divide by largest power of two: keep shifting right while even */
    while ((val & 1) == 0)
        val >>= 1;

    return val;
}
