/*
 * A334463  a(n) is the sum of all parts of all partitions of n into
 *          consecutive parts that differ by 3.
 * https://oeis.org/A334463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A117277(n)
 * Range:     u8 n <= 68, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117277(n) counts the partitions of n into decreasing consecutive parts
   that differ by 3.  Each such partition sums to n, so for every
   partition we add n to the total sum; thus a(n) = n * (#partitions). */

seq_t a(idx_t n)
{
    /* Count the number of partitions of n into consecutive parts
       with difference 3.  Each partition is of the form
           k, k-3, k-6, ..., k-3(m-1)
       with m ≥ 1 and all parts ≥ 1.  Its sum is n. */
    idx_t count = 0;
    if (n == 0) return (seq_t)0;    /* offset starts at 1, but harmless */

    /* 2*n as idx_t */
    idx_t two_n = n + n;   /* safer than shifting for very large n */

    /* Maximum number of parts m satisfies m*(3m-1)/2 ≤ n */
    idx_t m_max = 0;
    for (idx_t m = 1; ; ++m)
    {
        /* compute m*(3m-1)/2 */
        /* use idx_t arithmetic; intermediate fits for n up to 2^63 */
        idx_t lhs = m * ((idx_t)3 * m - 1) / 2;
        if (lhs > n) break;
        m_max = m;
    }

    for (idx_t m = 1; m <= m_max; ++m)
    {
        if (two_n % m != 0) continue;   /* m must divide 2n */
        idx_t d = two_n / m;            /* d = 2n/m */
        idx_t temp = d + (idx_t)3 * m - 3;
        if ((temp & 1) != 0) continue;  /* k must be integer: temp even */
        idx_t k = temp >> 1;            /* k = temp/2 */

        /* Verify positivity of last part: k - 3(m-1) ≥ 1  ==>  k ≥ 3m-2 */
        if (k < (idx_t)3 * m - 2) continue;

        /* All conditions satisfied: one valid partition found */
        ++count;
    }

    /* a(n) = n * count */
    return (seq_t)n * (seq_t)count;
}
