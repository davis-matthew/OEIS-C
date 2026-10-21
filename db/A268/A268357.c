/*
 * A268357  Highest power of 11 dividing n.
 * https://oeis.org/A268357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 11^valuation(n,11) (highest power of 11 dividing n)
 * Range:     u8 n <= 1330, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: n == 0 never occurs in the sequence but we handle it gracefully */
    if (n == 0)
        return 1;

    idx_t tmp = n;
    unsigned char exp = 0;
    while (tmp % 11 == 0) {
        tmp /= 11;
        ++exp;
    }

    seq_t result = 1;
    while (exp--)
        result *= (seq_t)11;

    return result;
}
