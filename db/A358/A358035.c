/*
 * A358035  a(n) = (8*n^3 + 12*n^2 + 4*n - 9)/3.
 * https://oeis.org/A358035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3 + 12*n^2 + 4*n - 9)/3
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    if (sizeof(seq_t) <= 8) {
        __uint128_t N = n;
        __uint128_t num =
            8 * N * N * N + 12 * N * N + 4 * N - 9;
        return (seq_t)(num / 3);
    } else {
        seq_t N = n;
        seq_t num =
            8 * N * N * N + 12 * N * N + 4 * N - 9;
        return num / 3;
    }
}
