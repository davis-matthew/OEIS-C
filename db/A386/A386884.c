/*
 * A386884  a(n) is the number of distinct four-cuboid combinations that fill
 *          an n X n X n cube using only strict cuboids.
 * https://oeis.org/A386884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4) * binom(floor((n-1)/2), 2)
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    /* k = floor((n-1)/2) */
    idx_t k = (n - 1) / 2;
    seq_t result = (seq_t)(n - 4) * (seq_t)k * (seq_t)(k - 1) / 2;
    return result;
}
