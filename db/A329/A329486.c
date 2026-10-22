/*
 * A329486  a(n) = 3*A006519(n)/2 + n/2 where A006519(n) is the highest power
 *          of 2 dividing n.
 * https://oeis.org/A329486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*A006519(n)+n)/2, where A006519(n) is the highest power
 *            of 2 dividing n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute the highest power of 2 dividing n. */
    seq_t lowbit = (seq_t)(n & (~(n - 1)));   /* same as n & -n for unsigned n */

    /* Compute the result using integer arithmetic. */
    return (3 * lowbit + (seq_t)n) / 2;
}
