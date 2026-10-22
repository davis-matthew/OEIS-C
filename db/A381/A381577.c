/*
 * A381577  Unique sequence of 0's, 1's, and 2's such that for any terms x and
 *          y with x < y, the subsequence of x's and y's becomes the
 *          Thue-Morse sequence after substitution x -> 0, y -> 1.
 * https://oeis.org/A381577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (popcount(floor(n/3))%2==0) ? (n%3) : (2-(n%3))
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int parity64(idx_t x)
{
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (int)(x & 1);
}

seq_t a(idx_t n)
{
    idx_t k = n / 3;
    idx_t r = n % 3;
    int p = parity64(k);
    return (seq_t)(p ? 2 - r : r);
}
