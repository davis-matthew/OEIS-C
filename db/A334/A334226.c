/*
 * A334226  a(n) is the number of times that the number 3*k+1 from the Collatz
 *          trajectory of n is greater than n.
 * https://oeis.org/A334226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd Collatz steps 3*k+1 that exceed the start
 *            value n
 * Range:     u8 n <= 26, u16 n <= 7902, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* defined for n ≥ 1, guard against 0 */
    }

    seq_t start = (seq_t)n;
    seq_t cur   = start;
    seq_t count = 0;

    while (cur != (seq_t)1) {
        /* test oddness by modulo 2 */
        if ((cur % (seq_t)2) != (seq_t)0) {        /* odd step  k → 3*k+1 */
            seq_t next = cur * (seq_t)3 + (seq_t)1;
            if (next > start) {
                ++count;
            }
            cur = next;
        } else {                                 /* even step  k → k/2 */
            cur = cur / (seq_t)2;
        }
    }

    return count;
}
