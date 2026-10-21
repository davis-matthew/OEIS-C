/*
 * A245836  Row sums in triangle A053398 (Kopper's Nim values).
 * https://oeis.org/A245836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0 a(2n) = 2 * (a(n) + n) a(2n+1) = a(n) + n (for n ≥ 1)
 *            This recurrence reproduces the OEIS sequence A245836
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n & 1) {          /* n is odd, n = 2k+1 */
        idx_t k = (n - 1) >> 1;
        return a(k) + (seq_t)k;
    } else {              /* n is even, n = 2k */
        idx_t k = n >> 1;
        return (seq_t)2 * (a(k) + (seq_t)k);
    }
}
