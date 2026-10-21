/*
 * A256756  a(n) = bitwise XOR of n and the reverse of n.
 * https://oeis.org/A256756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n XOR reverse(n) (decimal reverse of n)
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute decimal reverse of n using idx_t to avoid overflow in intermediate steps */
    idx_t t = n;
    idx_t rev_i = 0;
    while (t > 0) {
        rev_i = rev_i * 10 + (t % 10);
        t = t / 10;
    }
    /* cast to seq_t only after the full reverse is obtained */
    seq_t rev = (seq_t)rev_i;
    seq_t val = (seq_t)n ^ rev;
    return val;
}
