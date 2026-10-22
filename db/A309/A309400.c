/*
 * A309400  Irregular triangle read by rows in which row n lists in reverse
 *          order the partitions of n into equal parts.
 * https://oeis.org/A309400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A309400: flatten the irregular triangle of partitions
   of each integer into equal parts.  The algorithm iterates rows,
   using the sum of divisors σ(row) as the number of terms in the row,
   then locates the target index within that row. */

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; for n < 1 just return 0. */
    if (n == 0)
        return 0;

    /* cur holds the starting index of the current row. */
    idx_t cur = 1;
    /* row is the integer whose equal‑part partitions form the current row. */
    idx_t row = 1;

    /* Find the row containing n.  cur + sigma(row) is the first index
       after the current row. */
    for (;;) {
        /* Compute σ(row) = sum of divisors of row. */
        idx_t sigma = 0;
        for (idx_t d = 1; d <= row / d; ++d) {
            if (row % d == 0) {
                sigma += d;
                if (d != row / d)
                    sigma += row / d;
            }
        }

        /* If the target lies beyond this row, skip it. */
        if (cur + sigma > n)
            break;                   /* the desired term is in this row */
        cur += sigma;                /* move to next row */
        ++row;
    }

    /* offset is the position inside the found row (0‑based). */
    idx_t offset = n - cur;

    /* In the row, each divisor m of row gives the part size m repeated
       (row / m) times, in increasing order of m.  Find the part that
       corresponds to the offset. */
    for (idx_t m = 1; m <= row; ++m) {
        if (row % m != 0)
            continue;                /* m is not a divisor */
        idx_t cnt = row / m;         /* number of occurrences of m */
        if (offset < cnt)
            return (seq_t)m;         /* found the required term */
        offset -= cnt;
    }

    /* Control should never reach here. */
    return 0;
}
