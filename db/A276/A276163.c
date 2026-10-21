/*
 * A276163  a(n) is the maximum first-player score difference of a "Coins in a
 *          Row" game over all permutations of coins 1..n with both players
 *          using a minimax strategy.
 * https://oeis.org/A276163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n/2)^2 for even n = n for odd n with n ≡ 1 (mod 4) = n-1
 *            for odd n with n ≡ 3 (mod 4)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even n: (n/2)^2 */
    if ((n & 1ULL) == 0) {
        seq_t k = (seq_t)(n >> 1);   /* n/2 */
        return k * k;
    }
    /* odd n */
    if ((n & 3ULL) == 1) {          /* n ≡ 1 (mod 4) */
        return (seq_t)n;
    }
    /* n ≡ 3 (mod 4) */
    return (seq_t)(n - 1);
}
