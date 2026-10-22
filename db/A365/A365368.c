/*
 * A365368  First integer > n reached under iteration of map x ->
 *          (5/3)*round(x) when started at n, or -1 if no such integer is ever
 *          reached.
 * https://oeis.org/A365368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first integer > n reached by the iteration x0 = n,
 *            x_{k+1} = (5/3)*round(x_k) return the first integer > n for
 *            which the product (5/3)*round(x_k) is an integer; return -1 if
 *            unreachable
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;

    /* Immediate success if 5*n is a multiple of 3. */
    if (((target * 5) % 3) == 0)
        return (target * 5) / 3;

    /* Otherwise iterate: m_k = round(x_k), start with m_0 = n. */
    seq_t m = target;
    while (1)
    {
        /* m_next = round(5*m/3) = (5*m + 1)/3 for positive integers. */
        seq_t m_next = ((m * 5) + 1) / 3;

        /* If round result is divisible by 3, the next multiplication
           yields an integer > target. */
        if ((m_next % 3) == 0)
            return (m_next * 5) / 3;

        m = m_next;
    }

    /* In theory we never reach here; keep compiler happy. */
    return (seq_t)-1;
}
