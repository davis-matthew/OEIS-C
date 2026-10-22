/*
 * A387188  a(n) = s*(s+1)/2 + t + 1 where s = A160384(n) and t = A081603(n)
 *          are the number of nonzero and number of 2 digits of n in ternary.
 * https://oeis.org/A387188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = A000217(A160384(n)) + A081603(n) + 1 where A160384(n) is
 *            the number of nonzero ternary digits of n and A081603(n) is the
 *            number of 2's in the ternary expansion of n
 * Range:     u8 n <= 9411, u16 n <= 9411, u32 n <= 9411, u64 n <= 9411,
 *            u128 n <= 9411, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = 0;   /* number of nonzero digits */
    seq_t t = 0;   /* number of digits equal to 2 */
    idx_t x = n;   /* work on a copy of n */

    while (x > 0) {
        idx_t d = x % 3;
        if (d != 0) ++s;
        if (d == 2) ++t;
        x /= 3;
    }

    seq_t res = s * (s + 1) / 2 + t + 1;
    return res;
}
