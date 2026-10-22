/*
 * A306577  Last odd number reached by n before 1 through Collatz iteration,
 *          where a(n) = 1 when no other odd number is reached, or -1 if 1 is
 *          never reached.
 * https://oeis.org/A306577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = last odd number in the Collatz trajectory of n before 1
 *            (1 if no odd number other than 1 occurs; -1 if 1 is never
 *            reached)
 * Range:     u8 n <= 74, u16 n <= 3742, u32 n <= 6861, u64 n <= 6861,
 *            u128 n <= 6861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t last = 1;          /* default value when no odd greater than 1 is seen */

    if (cur == 1) return 1;  /* trivial case */

    while (cur != 1) {
        if (cur & 1) {        /* odd step */
            last = cur;
            cur = 3 * cur + 1;
        } else {             /* even step */
            cur >>= 1;
        }
    }
    return last;
}
