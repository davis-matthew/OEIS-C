/*
 * A263426  Permutation of the nonnegative integers: [4k+2, 4k+1, 4k, 4k+3,
 *          ...].
 * https://oeis.org/A263426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*floor((n+1)/4) - (n mod 4) + 2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/4), r = n mod 4 */
    idx_t k = n >> 2;      /* k = n / 4 */
    idx_t r = n & 3U;      /* r = n % 4 */

    /* mapping of n mod 4 to the offset in a(n) */
    static const seq_t map[4] = {2, 1, 0, 3};

    /* compute 4*k + map[r] safely in seq_t */
    seq_t res = ((seq_t)k << 2) + map[r];
    return res;
}
