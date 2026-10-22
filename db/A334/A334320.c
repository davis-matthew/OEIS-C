/*
 * A334320  Number of even integers in base n with exactly two distinct
 *          digits.
 * https://oeis.org/A334320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-2)/2 + 1 (if n is even), a(n) = (n-1)*(n-2)/2 (if n
 *            is odd) with a(0) = a(1) = 0. This counts the number of
 *            two‑digit base‑n numbers that use exactly two distinct digits
 *            and represent an even integer
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(0) = a(1) = 0 */

    if ((n & 1) == 0) {                  /* n is even */
        seq_t nn = (seq_t)n;
        return (nn * (nn - (seq_t)2)) / (seq_t)2 + (seq_t)1;
    } else {                             /* n is odd */
        seq_t nn = (seq_t)n;
        return ((nn - (seq_t)1) * (nn - (seq_t)2)) / (seq_t)2;
    }
}
