/*
 * A347717  Number of states of the minimal deterministic finite automaton
 *          that accepts ternary strings that represent numbers that are
 *          divisible by n.
 * https://oeis.org/A347717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n, if n mod 3 ≠ 0; otherwise a(3m) = a(m) + 1
 *            Equivalently, a(n) = n / 3^k + k where k = the largest power of
 *            3 dividing n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case – the sequence is defined for n ≥ 1. */
    if (n == 0) return (seq_t)0;

    idx_t m = n;    /* copy of n to strip factors of 3 */
    idx_t k = 0;    /* count of factors of 3 */

    /* Divide out all factors of 3 from n */
    while (m % 3 == 0) {
        m /= 3;
        k += 1;
    }

    /* Now n = 3^k * m with m not divisible by 3, so a(n) = m + k. */
    return (seq_t)m + (seq_t)k;
}
