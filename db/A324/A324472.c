/*
 * A324472  a(n) = 1000 mod n.
 * https://oeis.org/A324472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1000 mod n
 * Range:     u8 n <= 333, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety we guard against n == 0, though the offset starts at 1 */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)(1000ULL % n);
}
