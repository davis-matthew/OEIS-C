/*
 * A346877  Sum of the divisors, except for the largest, of the n-th odd
 *          number.
 * https://oeis.org/A346877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of positive divisors of (2*n-1) minus (2*n-1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A346877:  
   a(n) = sum_{d|m, d < m} d,  where m = 2*n-1 (the n-th odd number)  */
seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t m = 2 * n - 1;          /* the n-th odd number */
    seq_t sum = 0;                /* accumulated proper-divisor sum */

    /* iterate over all d such that 1 <= d <= sqrt(m) */
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            /* d is a divisor, other = m / d is the paired divisor */
            seq_t d_seq   = (seq_t)d;
            seq_t other   = (seq_t)(m / d);
            seq_t m_seq   = (seq_t)m;

            /* add d if it is not the largest divisor (i.e., not m) */
            if (d_seq != m_seq)
                sum += d_seq;

            /* add the paired divisor if distinct from d and not m */
            if (other != d_seq && other != m_seq)
                sum += other;
        }
    }

    return sum;
}
