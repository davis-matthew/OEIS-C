/*
 * A358016  a(n) is the largest k <= n-2 such that k^2 == 1 (mod n).
 * https://oeis.org/A358016
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k ≤ n-2 | k^2 ≡ 1 (mod n)}
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;
    for (idx_t k = n - 2; k > 0; --k) {
        if (((seq_t)k * (seq_t)k % (seq_t)n) == (seq_t)1)
            return (seq_t)k;
    }
    return (seq_t)1;  /* always reached via k = 1 */
}
