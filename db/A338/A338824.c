/*
 * A338824  Lexicographically earliest sequence of nonnegative integers such
 *          that for any distinct m and n, a(m) OR a(m+1) <> a(n) OR a(n+1)
 *          (where OR denotes the bitwise OR operator).
 * https://oeis.org/A338824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = * n-2, if n is even; * 0, if n ≡ 1 (mod 4); * 1, if n ≡ 3
 *            (mod 4); (n ≥ 1) * This implements the closed form from OEIS
 *            A338824
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* position 0 is outside the usual offset */

    if ((n & 1) == 0)             /* even indices */
        return (seq_t)n - (seq_t)2;

    /* odd indices: n % 4 == 1 or 3 */
    if ((n & 3) == 1)             /* n ≡ 1 (mod 4) */
        return (seq_t)0;
    else                          /* n ≡ 3 (mod 4) */
        return (seq_t)1;
}
