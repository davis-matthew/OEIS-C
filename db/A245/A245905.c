/*
 * A245905  Zero followed by the terms of A023705 arranged to give the unique
 *          path to the n-th node of a complete, rooted and ordered ternary
 *          tree.
 * https://oeis.org/A245905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1 let p = (n-2)/3 + 1, d = ((n-2)%3) + 1; then
 *            a(n) = 4*a(p) + d
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t result = 0;
    idx_t i = n;

    while (i > 1) {
        seq_t d = (seq_t)((i - 2) % 3 + 1);
        result = result * 4 + d;
        i = (i - 2) / 3 + 1;
    }

    return result;
}
