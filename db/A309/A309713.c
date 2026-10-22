/*
 * A309713  Sum of the odd parts appearing among the third largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sum of the odd parts appearing among the third largest parts
 * of the partitions of n into 4 parts.
 * Formula (from the OEIS entry):
 *   a(n) = Σ_{k=1}^{⌊n/4⌋} Σ_{j=k}^{⌊(n-k)/3⌋} Σ_{i=j}^{⌊(n-j-k)/2⌋} j·(j mod 2)
 * This function implements that triple‑sum directly, grouped by the
 * odd value of j (the third largest part) for efficiency.
 */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* maximum odd j satisfying j ≤ ⌊(n-k)/3⌋ for some k≥1 */
    idx_t j_max = n / 3;
    for (idx_t j = 1; j <= j_max; j += 2) {          /* j is odd */
        /* upper bound for k: must satisfy k ≤ n/4, k ≤ n-3j, and k ≤ j */
        idx_t kmax = n / 4;

        idx_t temp = n - 3 * j;                     /* n - 3j */
        if (temp < 1) continue;                     /* no possible k */

        if (temp < kmax) kmax = temp;
        if (j < kmax)   kmax = j;

        if (kmax < 1) continue;

        for (idx_t k = 1; k <= kmax; ++k) {
            /* remaining sum for the fourth part */
            idx_t m = n - j - k;                    /* n - j - k */
            /* guaranteed that m ≥ 2j because k ≤ n-3j */

            idx_t i_max = m / 2;                    /* floor((n-j-k)/2) */
            seq_t count_i = (seq_t)(i_max - j + 1); /* number of possible i */

            result += (seq_t)j * count_i;
        }
    }

    return result;
}
