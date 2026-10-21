/*
 * A241199  Numbers n such that 4 consecutive terms of binomial(n,k) satisfy a
 *          quadratic relation for 0 <= k <= n/2.
 * https://oeis.org/A241199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*((n+1)/2)^2 + 8*((n+1)/2) + 3 if n is odd a(n) =
 *            3*(n/2)^2 + 10*(n/2) + 6 if n is even (equivalent to the closed
 *            form given in OEIS A241199)
 * Range:     u8 n <= 14, u16 n <= 292, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        /* even n = 2k */
        seq_t k = ((seq_t)n) / 2;
        return 3 * k * k + 10 * k + 6;
    } else {
        /* odd n = 2k-1  -> k = (n+1)/2 */
        seq_t k = ((seq_t)n + 1) / 2;
        return 3 * k * k + 8 * k + 3;
    }
}
