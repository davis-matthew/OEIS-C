/*
 * A364388  Number of divisors of n of the form 5*k+1 that are at most
 *          sqrt(n).
 * https://oeis.org/A364388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of divisors d of n such that d ≡ 1 (mod 5) and d ≤
 *            sqrt(n)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t d = 1; (__int128)d * d <= n; ++d) {
        if (n % d == 0) {
            if (d % 5 == 1) {
                ++cnt;
            }
        }
    }
    return cnt;
}
