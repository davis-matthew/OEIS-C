/*
 * A260112  Minimal number of steps to get from 0 to n by (a) adding 1 or (b)
 *          multiplying by 4.
 * https://oeis.org/A260112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of base-4 digits of n + length of base-4
 *            representation of n - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: a(n) = A053737(n) + A110591(n) - 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t weight = 0;
    seq_t len = 0;
    idx_t x = n;
    while (x > 0) {
        weight += (seq_t)(x % 4);
        x /= 4;
        len++;
    }
    return weight + len - 1;
}
