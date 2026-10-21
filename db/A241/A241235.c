/*
 * A241235  a(n) = number of times n appears in A006949.
 * https://oeis.org/A241235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let n = 2^k * m with m odd. Then a(1) = 3; if m == 1 (i.e., n is
 *            a power of 2): a(n) = k + 2; otherwise: a(n) = k + 1. This
 *            satisfies the OEIS terms for A241235
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)                     /* special base case */
        return (seq_t)3;

    idx_t k = 0;                    /* count trailing zeros (v2(n)) */
    while ((n & 1UL) == 0) {
        k++;
        n >>= 1;                    /* remove factors of 2 */
    }

    if (n == 1)                    /* n was a power of 2 */
        return (seq_t)(k + 2);

    return (seq_t)(k + 1);         /* odd part > 1 */
}
