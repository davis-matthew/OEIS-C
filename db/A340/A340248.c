/*
 * A340248  Sum of the second largest parts s of the partitions of n into 4
 *          parts q,r,s,t such that 1 <= q <= r <= s <= t and q + r + s > t.
 * https://oeis.org/A340248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the second largest part s over partitions of n
 *            into four parts q ≤ r ≤ s ≤ t with 1 ≤ q and q + r + s > t
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined above. */
seq_t a(idx_t n)
{
    /* For n < 4 no such partitions exist. */
    if (n < 4)
        return 0;

    seq_t result = 0;
    /* s (the second largest part) ranges from 1 to floor((n-2)/2). */
    for (idx_t s = 1; s <= (n - 2) / 2; ++s)
    {
        /* t must satisfy s ≤ t < n/2 and q + r ≥ 2, i.e. t ≤ n - s - 2. */
        idx_t ub1 = (n - 1) / 2;      /* t < n/2  */
        idx_t ub2 = n - s - 2;        /* q + r ≥ 2 */
        idx_t ub = (ub1 < ub2) ? ub1 : ub2;
        idx_t lb = s;

        if (lb > ub)
            continue;

        idx_t count_s = 0;            /* number of valid (q,r) for current t */
        for (idx_t t = lb; t <= ub; ++t)
        {
            idx_t S = n - s - t;      /* q + r */
            if (S < 2)
                continue;

            /* q must satisfy max(1, S - s) ≤ q ≤ min(S/2, s). */
            idx_t low  = (S > s) ? (S - s) : 1;
            idx_t high = S / 2;
            if (high > s)
                high = s;

            if (high >= low)
                count_s += (high - low + 1);
        }

        result += (seq_t)s * (seq_t)count_s;
    }

    return result;
}
