/*
 * A393106  Halting time for Wolfram's 2-state 2-symbol Turing machine 261
 *          when started with n on the tape.
 * https://oeis.org/A393106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 if n % 4 == 0, otherwise a(n) = a(n/2) + 2.
 *            Equivalently, a(n) = 3 + 2 * (number of times n is shifted right
 *            until it becomes divisible by 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t inc = 0;
    while ((n & 3) != 0) {        /* n % 4 != 0 */
        inc += (seq_t)2;
        n >>= 1;                  /* divide by 2 */
    }
    return (seq_t)3 + inc;
}
