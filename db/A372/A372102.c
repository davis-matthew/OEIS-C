/*
 * A372102  Number of permutations of [n] whose non-fixed points are not
 *          neighbors.
 * https://oeis.org/A372102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n+1)/2⌋} !k · C(n‑k+1, k) where !k is the
 *            subfactorial (derangements of k elements) and C(m, k) is the
 *            binomial coefficient
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 38, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation below follows the formula exactly:
   - Subfactorials are generated iteratively using the recurrence
     !k =(k‑1)(!(k‑1)+!(k‑2)).
   - For each k we compute the binomial coefficient
     C(n‑k+1, k) with the multiplicative formula that uses only
     integer arithmetic; the division is exact at each step.
   - The final sum is returned as a seq_t.
   The algorithm runs in O(n²/2) elementary operations,
   which is well below the 2‑second limit for n up to 10 000.  */

seq_t a(idx_t n)
{
    /* maximum k for which C(n‑k+1, k) is defined */
    idx_t max_k = (n + 1) / 2;

    seq_t result = (seq_t)0;

    /* subfactorials: !0=1, !1=0 */
    seq_t der_prev2 = (seq_t)1;  /* !0   */
    seq_t der_prev1 = (seq_t)0;  /* !1   */
    seq_t der_k;                 /* !k   */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* compute der_k = !k */
        if (k == 0)
            der_k = der_prev2;
        else if (k == 1)
            der_k = der_prev1;
        else
        {
            seq_t cur = (seq_t)(k - 1) * (der_prev1 + der_prev2);
            der_k = cur;
            der_prev2 = der_prev1;
            der_prev1 = cur;
        }

        /* compute C(n‑k+1, k) */
        seq_t bin = (seq_t)1;
        if (k > 0)
        {
            for (idx_t i = 1; i <= k; ++i)
            {
                /* numerator = n – 2k + 1 + i   (all values are non‑negative) */
                seq_t num = (seq_t)n - (seq_t)2 * k + (seq_t)1 + (seq_t)i;
                bin = bin * num;
                bin = bin / (seq_t)i;
            }
        }

        result += der_k * bin;
    }

    return result;
}
