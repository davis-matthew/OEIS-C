/*
 * A337509  Number of partitions of n into two distinct parts (s,t), such that
 *          (t-s) | n, and where n/(t-s) <= s < t.
 * https://oeis.org/A337509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (s,t) with s<t, s+t=n, (t-s)|n, n/(t-s)
 *            <= s. Equivalent: for each divisor d of n with d<n, let
 *            s=(n-d)/2; require n-d even, s>=1, and n/d <= s
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 0 for n<1, although the sequence is defined from n=1 onward */
    if (n < 1) return 0;

    seq_t count = 0;

    /* Enumerate all divisors of n.  n is at most 64 bit.  */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        /* First divisor d */
        if (d != n)                /* exclude d == n because s would be 0 */
        {
            if (((n - d) & 1) == 0) /* n-d even */
            {
                seq_t s = (seq_t)(n - d) / 2; /* exact division */
                if (s >= 1)
                {
                    /* n/d is exact because d | n */
                    idx_t ddiv = n / d;
                    if ((seq_t)ddiv <= s)
                        ++count;
                }
            }
        }

        /* Second divisor d2 = n/d, skip if it equals d */
        if (d != n / d && (n / d) != n)
        {
            idx_t d2 = n / d;
            if (((n - d2) & 1) == 0)
            {
                seq_t s = (seq_t)(n - d2) / 2;
                if (s >= 1)
                {
                    idx_t ddiv = n / d2; /* which is d */
                    if ((seq_t)ddiv <= s)
                        ++count;
                }
            }
        }
    }

    return count;
}
