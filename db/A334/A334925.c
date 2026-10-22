/*
 * A334925  G.f.: Sum_{k>=1} x^(k*(k^2 + 1)/2) / (1 - x^(k*(k^2 + 1)/2)).
 * https://oeis.org/A334925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d = k*(k*k + 1)/2 for
 *            some integer k >= 1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1;; ++k) {
        unsigned __int128 kk = k;
        unsigned __int128 dk = kk * (kk * kk + 1) / 2;  /* d = k*(k^2+1)/2 */
        if ((unsigned __int128)n < dk) break;          /* further d are larger than n */
        if (n % (idx_t)dk == 0) ++count;               /* d divides n */
    }
    return count;
}
