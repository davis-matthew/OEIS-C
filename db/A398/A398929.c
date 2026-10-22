/*
 * A398929  Number of sets R(k) that contain n, where R(k) is the set of k
 *          consecutive integers centered at k (excluding k if k is even,
 *          including k if k is odd).
 * https://oeis.org/A398929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/3) + (n mod 2)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn + (nn / 3) + (nn & 1);
}
