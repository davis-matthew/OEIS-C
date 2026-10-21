/*
 * A290561  a(n) = n + cos(n*Pi/2).
 * https://oeis.org/A290561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + cos(n*Pi/2) (integer implementation)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n mod 4 using bitwise AND (since n is unsigned) */
    idx_t r = n & 3;
    seq_t sn = (seq_t)n;          /* cast index to sequence type once */

    if (r == 0)          /* n ≡ 0 (mod 4): cos = 1  */
        return sn + 1;   /* a(n) = n + 1 */
    else if (r == 2)     /* n ≡ 2 (mod 4): cos = -1 */
        return sn - 1;   /* a(n) = n - 1 */
    else                 /* n ≡ 1 or 3 (mod 4): cos = 0  */
        return sn;      /* a(n) = n */
}
