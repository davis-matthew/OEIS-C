/*
 * A385989  a(n) is the least m > n such that 2^n and 2^m are congruent modulo
 *          n.
 * https://oeis.org/A385989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A007733(n) where A007733(n) is the multiplicative
 *            order of 2 modulo the odd part of n (with A007733(1)=1). For n =
 *            2^k this simplifies to a(n) = n + 1
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: n is a power of two.  Then a(n) = n + 1. */
    if ((n & (n - 1)) == 0)
        return (seq_t)n + 1;

    /* Compute the odd part of n. */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* Find the multiplicative order of 2 modulo odd. */
    idx_t order = 1;
    idx_t cur = 2 % odd;
    while (cur != 1)
    {
        cur = (cur * 2) % odd;
        ++order;
    }

    /* a(n) = n + order.  cast order to seq_t as needed. */
    return (seq_t)n + (seq_t)order;
}
