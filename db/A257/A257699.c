/*
 * A257699  Number of steps from n to 0 using this algorithm: x -> floor(r*x)
 *          if x is odd, and x -> floor(x/r) if x is even, where r = 5/2.
 * https://oeis.org/A257699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps needed to reach 0 by repeatedly applying:
 *            x -> floor(5*x/2) if x is odd, and x -> floor(2*x/5) if x is
 *            even
 * Range:     u8 n <= 10, u16 n <= 908, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t steps = 0;
    while (x > 0) {
        if (x & 1)          // odd
            x = (x * 5) / 2;
        else                // even
            x = (x * 2) / 5;
        ++steps;
    }
    return steps;
}
