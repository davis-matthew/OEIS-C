/*
 * A284805  Write in base k, complement, reverse. Case k = 7.
 * https://oeis.org/A284805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = \sum_{i=0}^{L-1} (6 - d_i) \cdot 7^i, where d_i are the
 *            base‑7 digits of n
 * Range:     u8 n <= 48, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    seq_t res = 0;
    idx_t x = n;
    while (x > 0) {
        unsigned d = x % 7;
        unsigned c = 6 - d;
        res = res * 7 + (seq_t)c;
        x /= 7;
    }
    return res;
}
