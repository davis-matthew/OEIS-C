/*
 * A348647  Irregular table read by rows; the n-th row contains the lengths of
 *          the runs of consecutive terms with the same parity in the n-th row
 *          of Pascal's triangle (A007318).
 * https://oeis.org/A348647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 6304, u16 n <= 6304, u32 n <= 6304, u64 n <= 6304,
 *            u128 n <= 6304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Irregular table read by rows – sequence A348647.
   Generates rows of Pascal's triangle, records runs of equal parity
   (odd or even), and returns the n‑th run length in the flattened
   list of all rows.  The function uses only integer arithmetic and
   works for all builds of seq_t and idx_t. */

seq_t a(idx_t n)
{
    idx_t offset = n;                     /* number of runs to skip */

    for (idx_t r = 0; ; ++r) {           /* iterate over Pascal rows */
        /* first coefficient (k == 0) is always odd */
        idx_t prev_parity = 1;
        idx_t run_len     = 1;

        /* process remaining coefficients of the row */
        for (idx_t k = 1; k <= r; ++k) {
            idx_t parity = ((k & ~r) == 0) ? 1 : 0;

            if (parity == prev_parity) {
                ++run_len;                 /* continue current run */
            } else {
                /* current run ends here */
                if (offset == 0) {
                    return (seq_t)run_len;
                }
                --offset;                   /* skip this run */
                prev_parity = parity;       /* start new run */
                run_len = 1;
            }
        }

        /* handle the final run of the row */
        if (offset == 0) {
            return (seq_t)run_len;
        }
        --offset;
    }
}
