/*
 * A348706  Delete all 0's from ternary expansion of n.
 * https://oeis.org/A348706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Delete all 0's from ternary expansion of n
 * Range:     u8 n <= 363, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    idx_t m      = n;    /* work on a copy of n */

    while (m > 0) {
        unsigned digit = m % 3;   /* least significant ternary digit */
        m /= 3;
        if (digit) {
            result += (seq_t)digit * power;
            power  *= 3;
        }
    }
    return result;
}
