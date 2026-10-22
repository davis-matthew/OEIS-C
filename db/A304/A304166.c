/*
 * A304166  a(n) = 972*n^2 - 1224*n + 414 with n > 0.
 * https://oeis.org/A304166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 972*n^2 - 1224*n + 414 (n > 0)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result = 972 * N * N;
    result -= 1224 * N;
    result += 414;
    return result;
}
