/*
 * A257542  Square-sum pairs: Numbers n such that 0,1, ..., 2n-1 can be
 *          partitioned into n pairs, where each pair adds up to a perfect
 *          square.
 * https://oeis.org/A257542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3 for n ≥ 5. Initial values: a(1)=1, a(2)=4, a(3)=5,
 *            a(4)=7
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)7;
    /* For n ≥ 5 the sequence follows a(n) = n + 3. */
    return (seq_t)n + (seq_t)3;
}
