/*
 * A266720  Binary representation of the middle column of the "Rule 59"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10009 * 10^n / 9900). Equivalent recurrence: a(n) =
 *            10*a(n-1)+a(n-2)-10*a(n-3) for n>4. This implementation builds
 *            the decimal digit pattern directly: digits[0] = 1, digits[1] =
 *            0, digits[2] = 1, digits[3] = 1, and for i >= 4, digits[i] = (i
 *            is odd ? 1 : 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 0; ; ++i) {
        unsigned digit;
        if (i == 0)          digit = 1;
        else if (i == 1)     digit = 0;
        else if (i == 2)     digit = 1;
        else if (i == 3)     digit = 1;
        else                 digit = (i & 1) ? 1 : 0;   /* i >= 4 */

        val = val * (seq_t)10 + (seq_t)digit;

        if (i == n)
            break;
    }
    return val;
}
