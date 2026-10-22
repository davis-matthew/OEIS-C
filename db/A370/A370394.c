/*
 * A370394  Number of steps to reach 0 by applying the map f(x) = 5x + 1 if x
 *          is odd and floor(x/3) if x is even, or -1 if 0 is never reached.
 * https://oeis.org/A370394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reach 0 under map f(x) = 5x+1 if x is
 *            odd, floor(x/3) if x is even. (Conjectured that every n
 *            eventually reaches 0.)
 * Range:     u8 n <= 26, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t steps = 0;
    seq_t cur = (seq_t)n;
    while (cur != 0) {
        if ((cur & 1) != 0) {
            cur = 5 * cur + 1;
        } else {
            cur /= 3;   /* floor division by 3 */
        }
        ++steps;
    }
    return steps;
}
