/*
 * A365807  a(n) = 1 if A163511(n) is a square, 0 otherwise.
 * https://oeis.org/A365807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A163511(n) is a square, 0 otherwise. Recurrence:
 *            a(0)=1, a(1)=0, a(2)=1; for n>2, if n is odd → a(n) =
 *            a((n‑1)/2); else if n % 4 == 0 → a(n) = a(n/4); otherwise → a(n)
 *            = 0
 * Range:     u8 n <= 9866, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    if ((n & 1) == 1)               /* n is odd */
        return a((n - 1) >> 1);
    if ((n & 3) == 0)               /* n ≡ 0 (mod 4) */
        return a(n >> 2);

    return (seq_t)0;
}
