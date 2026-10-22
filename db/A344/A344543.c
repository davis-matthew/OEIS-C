/*
 * A344543  Lexicographically earliest sequence S of distinct positive terms
 *          such that the product of the last k digits of S is even, k being
 *          the rightmost digit of a(n).
 * https://oeis.org/A344543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 for n = 1; a(n) = n + 2 * floor((n-2)/8) for n >= 2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t nn = (seq_t)n;          /* cast index to seq_t for arithmetic */
    seq_t m  = (seq_t)(n - 2);    /* n-2 is non‑negative because n>=2 */
    seq_t add = 2 * (m / 8);      /* floor division by 8 */
    return nn + add;
}
