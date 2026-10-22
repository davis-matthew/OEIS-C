/*
 * A355319  Maximal GCD of four positive integers with sum n.
 * https://oeis.org/A355319
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest divisor of n that is ≤ n/4
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    idx_t limit = n / 4;
    for (idx_t d = limit; d != 0; --d) {
        if (n % d == 0)
            return (seq_t)d;
    }
    return 0; /* unreachable: 1 always divides n */
}
