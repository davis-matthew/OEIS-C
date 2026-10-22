/*
 * A306285  Numbers congruent to 4 or 21 mod 26.
 * https://oeis.org/A306285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n - 9 for n odd, 13*n - 5 for n even
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before multiplying to avoid overflow in uint64_t */
    seq_t v = (seq_t)13 * n;
    if (n & 1)   /* n is odd */
        return v - 9;
    else         /* n is even */
        return v - 5;
}
