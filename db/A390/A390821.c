/*
 * A390821  a(n) is the permanent of the unipotent Sylvester-Kac tridiagonal
 *          matrix of order n having main diagonal [0, 2, ..., 2*(n-1)],
 *          superdiagonal [1, 2, ..., n-1], and [subdiagonal 1-n, 2-n, ...,
 *          -1].
 * https://oeis.org/A390821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Permanent of the unipotent Sylvester‑Kac tridiagonal matrix.
   The sequence a(n) is the permanent of the n×n matrix whose
   main diagonal is [0,2,…,2(n‑1)],
   super‑diagonal is [1,2,…,n‑1] and
   sub‑diagonal is [1‑n,2‑n,…,‑1].
   The permanent can be computed by a simple O(n) dynamic programme
   that keeps track of which of the two columns that the current
   row may still match to are already occupied.  */

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention */
    if (n == 0)
        return (seq_t)1;

    /* dp[mask] – permanent of the processed prefix where
       mask  bit0 = 1 if the current column is already matched,
       bit1 = 1 if the next column (i+1) is already matched
               (this can only come from a previous super‑diagonal).  */
    seq_t dp[4] = {0};
    seq_t newdp[4];

    dp[0] = (seq_t)1;  /* before any rows, no column is occupied */

    for (idx_t i = 1; i <= n; ++i)
    {
        newdp[0] = newdp[1] = newdp[2] = newdp[3] = (seq_t)0;

        for (int mask = 0; mask < 4; ++mask)
        {
            seq_t cur = dp[mask];
            if (cur == (seq_t)0)
                continue;

            int occ_i_1 = mask & 1;          /* column (i‑1) matched? */
            int occ_i   = (mask >> 1) & 1;   /* column  i   matched? */

            /* Sub‑diagonal: i → i‑1, only if i > 1 and column (i‑1) free. */
            if (i > 1 && occ_i_1 == 0)
            {
                seq_t weight_sub = (seq_t)(i - 1) - (seq_t)n;   /* (i‑1) – n */
                int   nextMask   = occ_i;   /* column i keeps its previous status */
                newdp[nextMask] += cur * weight_sub;
            }

            /* Diagonal: i → i, only if column i is free. */
            if (occ_i == 0)
            {
                seq_t weight_diag = (seq_t)2 * (seq_t)(i - 1);   /* 2·(i‑1) */
                int   nextMask_diag = occ_i | 1;                 /* column i now matched */
                newdp[nextMask_diag] += cur * weight_diag;

                /* Super‑diagonal: i → i+1, only if i < n. */
                if (i < n)
                {
                    seq_t weight_super = (seq_t)i;                /* i */
                    int   nextMask_super = occ_i | 2;              /* column i+1 now matched */
                    newdp[nextMask_super] += cur * weight_super;
                }
            }
        }

        /* Copy newdp → dp for the next iteration. */
        dp[0] = newdp[0];
        dp[1] = newdp[1];
        dp[2] = newdp[2];
        dp[3] = newdp[3];
    }

    /* After processing all rows, column n must be matched,
       i.e., mask 01 (bit0==1, bit1==0). */
    return dp[1];
}
