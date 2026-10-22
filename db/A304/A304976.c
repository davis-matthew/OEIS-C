/*
 * A304976  Number of achiral color patterns (set partitions) for a row or
 *          loop of length n using exactly 6 colors (sets).
 * https://oeis.org/A304976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n==0 mod 2] * (S2(n/2+3,6) - 3*S2(n/2+2,6) -
 *            8*S2(n/2+1,6) + 16*S2(n/2,6)) + [n==1 mod 2] * (3*S2((n+5)/2,6)
 *            - 17*S2((n+3)/2,6) + 20*S2((n+1)/2,6))
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* S2(n,k) denotes the Stirling number of the second kind. */

seq_t a(idx_t n)
{
    /* Determine the maximum n for which a Stirling number is needed. */
    idx_t max_needed;
    if ((n & 1) == 0) {                 /* n even */
        max_needed = (n >> 1) + 3;      /* m+3 where m = n/2 */
    } else {                            /* n odd */
        max_needed = (n + 5) >> 1;      /* (n+5)/2 */
    }

    /* Indices of the Stirling numbers needed for the formula. */
    idx_t m_even   = n >> 1;
    idx_t idx0 = m_even;           /* S2(m,6)   */
    idx_t idx1 = m_even + 1;       /* S2(m+1,6) */
    idx_t idx2 = m_even + 2;       /* S2(m+2,6) */
    idx_t idx3 = m_even + 3;       /* S2(m+3,6) */

    idx_t idxb1 = (n + 5) >> 1;     /* S2((n+5)/2,6) */
    idx_t idxb2 = (n + 3) >> 1;     /* S2((n+3)/2,6) */
    idx_t idxb3 = (n + 1) >> 1;     /* S2((n+1)/2,6) */

    /* Temporary storage for values of S2(i,6) at the required indices. */
    seq_t A0 = 0, A1 = 0, A2 = 0, A3 = 0;
    seq_t B1 = 0, B2 = 0, B3 = 0;

    /* Only the column k=0..6 of the current Stirling row is needed. */
    seq_t prev[7];
    prev[0] = (seq_t)1;            /* S2(0,0) = 1 */
    for (idx_t k = 1; k <= 6; ++k) prev[k] = (seq_t)0;   /* S2(0,k) = 0 for k>0 */

    for (idx_t i = 0; i <= max_needed; ++i) {
        /* Capture S2(i,6) if the current index is required. */
        if (i == idx0) A0 = prev[6];
        if (i == idx1) A1 = prev[6];
        if (i == idx2) A2 = prev[6];
        if (i == idx3) A3 = prev[6];
        if (i == idxb1) B1 = prev[6];
        if (i == idxb2) B2 = prev[6];
        if (i == idxb3) B3 = prev[6];

        /* Compute the next row using the recurrence:
           S2(n,k) = k*S2(n-1,k) + S2(n-1,k-1). */
        if (i < max_needed) {
            for (idx_t k = 6; k >= 1; --k) {
                prev[k] = (seq_t)k * prev[k] + prev[k-1];
            }
            prev[0] = (seq_t)0;
        }
    }

    /* Assemble the result from the captured Stirling numbers. */
    seq_t result;
    if ((n & 1) == 0) {   /* even n */
        result = A3 - (seq_t)3 * A2 - (seq_t)8 * A1 + (seq_t)16 * A0;
    } else {              /* odd n */
        result = (seq_t)3 * B1 - (seq_t)17 * B2 + (seq_t)20 * B3;
    }

    return result;
}
