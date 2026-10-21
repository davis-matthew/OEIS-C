/*
 * A268354  Highest power of 7 dividing n.
 * https://oeis.org/A268354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 7^valuation(n,7) – highest power of 7 dividing n
 * Range:     u8 n <= 342, u16 n <= 9905, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 according to the offset.  For n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Find v = exponent of 7 in the prime factorisation of n. */
    idx_t m = n;
    idx_t v = 0;
    while (m % (idx_t)7 == 0) {
        v++;
        m /= (idx_t)7;
    }

    /* Compute 7^v with simple repeated multiplication. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < v; ++i)
        result *= (seq_t)7;

    return result;
}
