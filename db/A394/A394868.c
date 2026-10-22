/*
 * A394868  a(n) is the final number of steps reached by Alice and Bob, in the
 *          optimization game on an n X n grid described in comments.
 * https://oeis.org/A394868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 0 a(2) = 3 for n >= 3: a(n) = 4*n - 4 if n is even a(n) =
 *            4*n - 8 if n is odd This is a closed form that reproduces the
 *            known terms of OEIS sequence A394868
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 3;
    seq_t res = (seq_t)n * 4;  /* 4*n */
    res -= 4;
    if (n & 1)   /* odd n */
        res -= 4;
    return res;
}
