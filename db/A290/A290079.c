/*
 * A290079  Characteristic function for A249721: a(n) = 1 if there are either
 *          no 1-digits at all in base-3 representation of n, or if there are
 *          exactly two 1's next to each other, a(n) = 0 in any other cases.
 * https://oeis.org/A290079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is even and 9 does not divide C(n,n/2), otherwise
 *            0
 * Range:     u8 n <= 8769, u16 n <= 8769, u32 n <= 8769, u64 n <= 8769,
 *            u128 n <= 8769, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* odd n → 0 */
    if (n & 1U)
        return (seq_t)0;

    /* n is even: consider m = n/2 */
    idx_t m     = n >> 1U;   /* n/2 */
    idx_t carry = 0U;
    seq_t carry_count = 0U;

    /* Count carries when adding m + m in base 3 */
    while (m > 0) {
        idx_t digit   = m % 3U;          /* current trit of m */
        idx_t sum     = digit * 2U + carry;
        idx_t carry_out = sum / 3U;
        if (carry_out)
            carry_count++;
        carry = carry_out;
        m     /= 3U;
    }

    /* If number of carries is at most 1 then 9 does not divide C(n,n/2) */
    return (carry_count <= 1U) ? (seq_t)1 : (seq_t)0;
}
