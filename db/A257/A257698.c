/*
 * A257698  Number of steps from n to 1 using this algorithm: x -> floor(r*x)
 *          if x is odd, and x -> floor(x/r) if x is even, where r = 3/2.
 * https://oeis.org/A257698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps to reach 1 from n using the rule * if x
 *            is odd : x -> floor(3x/2) * if x is even : x -> floor(2x/3)
 * Range:     u8 n <= 64, u16 n <= 8648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x      = (seq_t)n;
    seq_t steps  = 0;

    while (x != (seq_t)1) {
        if ((x & 1) == 0) {          /* even */
            seq_t q = x / 3;         /* q = floor(x/3)   */
            seq_t r = x % 3;         /* r = x mod 3      */
            /* floor(2x/3) = 2*q + floor(2r/3) and floor(2r/3) is 1 only when r==2   */
            x = q * 2 + (r == 2);
        } else {                     /* odd  */
            /* floor(3x/2) = x + floor(x/2) for odd x */
            x = x + (x >> 1);
        }
        ++steps;
    }

    return steps;
}
