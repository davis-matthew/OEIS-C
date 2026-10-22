/*
 * A387312  a(n) is the action of "Rule 30" elementary cellular automaton on
 *          the binary representation of n if the cells may only expand into
 *          the significant bit.
 * https://oeis.org/A387312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n OR 2*n) XOR floor(n/2)
 * Range:     u8 n <= 127, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn | (nn << 1)) ^ (nn >> 1);
}
