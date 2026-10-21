/*
 * A244314  Nonnegative integers m such that the Semigreedy Catalan
 *          representation A244159(m) contains at least one zero.
 * https://oeis.org/A244314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 3378, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A244314.
   a(0) = 0.  For n ≥ 1, the non‑zero terms are
   the integers m that are in the ranges
        [ C_k ,  C_k + S_{k-1} - 1 ]
   where C_k is the k‑th Catalan number
   and S_{k-1} = Σ_{j=1}^{k-1} C_j.
   The algorithm finds the appropriate block k by subtracting
   the block lengths S_{k-1} until the remaining index lies
   inside the current block, and then returns the offset
   inside that block.  No other OEIS utilities are required. */

seq_t a(idx_t n)
{
    /*  Special case: a(0) = 0. */
    if (n == 0)
        return (seq_t)0;

    /*  Convert the 0‑based index into an index of the zero‑containing
        numbers (excluding the special 0).  */
    seq_t target = (seq_t)(n - 1);

    /*  Catalan numbers and prefix sums are generated on the fly.
        C_1 = 1, C_2 = 2, C_{k+1} = C_k * (4(k+1)-2) / (k+2)  (k ≥ 1). */

    idx_t k = 2;                         /* current block index (k ≥ 2) */
    seq_t catalan_prev = (seq_t)1;        /* C_1 */
    seq_t catalan_curr = (seq_t)2;        /* C_2 */
    seq_t sum_first = catalan_prev;      /* S_{k-1} for current k (k=2) */
    seq_t cumulative = (seq_t)0;          /* number of zeros processed so far */

    while (1)
    {
        seq_t block_len = sum_first;      /* S_{k-1} */

        if (target < cumulative + block_len)
        {
            /*  The desired term lies in the current block.  */
            seq_t offset = target - cumulative;
            return catalan_curr + offset;
        }

        /*  Skip the current block and prepare the next one.  */
        cumulative += block_len;
        sum_first += catalan_curr;            /* S_k for next iteration */

        /*  Compute C_{k+1} from C_k. */
        idx_t next_k = k + 1;
        /*  C_{next_k} = C_k * (4*next_k - 2) / (next_k + 1)  */
        seq_t numerator   = catalan_curr * ((seq_t)(4 * next_k - 2));
        seq_t denominator = (seq_t)(next_k + 1);
        catalan_curr = numerator / denominator;

        k = next_k;
    }
}
