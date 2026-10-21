/*
 * A288023  Number of steps to reach 1 in the Collatz 3x+1 problem starting
 *          with the n-th triangular number, or -1 if 1 is never reached.
 * https://oeis.org/A288023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Collatz steps to reach 1 starting from
 *            n*(n+1)/2
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t steps = 0;
    while (t != 1) {
        if (t & 1) {
            t = 3 * t + 1;
        } else {
            t >>= 1;
        }
        ++steps;
    }
    return steps;
}
