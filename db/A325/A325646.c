/*
 * A325646  Number of separable partitions of n in which the number of
 *          distinct (repeatable) parts is 2.
 * https://oeis.org/A325646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unordered pairs {a,b} of distinct positive
 *            integers and multiplicities (k,l) with k≥1,l≥1, |k-l|≤1 such
 *            that k*a + l*b = n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Early termination for the trivial cases */
    if (n < 3)
        return (seq_t)0;

    seq_t count = 0;

    for (idx_t a = 1; a <= n / 2; ++a) {
        /* b must be greater than a and a + b <= n for any admissible partition */
        for (idx_t b = a + 1; b <= n - a; ++b) {
            idx_t sum_ab = a + b;   /* a and b are distinct */
            /* Equal multiplicities: k = l = m */
            if (n % sum_ab == 0) {
                idx_t m = n / sum_ab;
                if (m >= 1)
                    count += (seq_t)1;
            }
            /* Case a has one extra part: (m+1)*a + m*b = n  */
            idx_t rem = n - a;
            if (rem % sum_ab == 0) {
                idx_t m = rem / sum_ab;
                if (m >= 1)
                    count += (seq_t)1;
            }
            /* Case b has one extra part: m*a + (m+1)*b = n  */
            rem = n - b;
            if (rem % sum_ab == 0) {
                idx_t m = rem / sum_ab;
                if (m >= 1)
                    count += (seq_t)1;
            }
        }
    }
    return count;
}
