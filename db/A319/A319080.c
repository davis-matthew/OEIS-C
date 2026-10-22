/*
 * A319080  Sequence, read by rows, encoding an infinite sequence of
 *          diamond-shaped arrays of nonnegative integers (see example).
 * https://oeis.org/A319080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is obtained by flattening successive diamond–shaped
 * arrays of non‑negative integers.  Let r be the radius of the figure
 * containing the n‑th term.  The figure consists of all cells (x,y)
 * with |x|+|y| ≤ r.  The value at each cell is   r – (|x|+|y|).
 * The terms are obtained row by row with y increasing from –r to +r
 * and, for each row, x increasing from –(r−|y|) to +(r−|y|). */

seq_t a(idx_t n)
{
    /* Find the figure radius r such that S(r‑1) ≤ n < S(r),
     * where S(r) = ∑_{k=0}^{r} (2k²+2k+1).  The computation uses
     * only 64‑bit arithmetic, which is safe for n up to 2^64‑1.*/
    idx_t r = 0;
    idx_t cumulative = 0;                 /* S(r‑1) */
    for (;;)
    {
        idx_t figure_size = 2 * r * r + 2 * r + 1;   /* 2r²+2r+1 */
        if (cumulative + figure_size > n)
            break;
        cumulative += figure_size;
        ++r;
    }
    idx_t offset = n - cumulative;              /* 0‑based inside the figure */

    /* Convert r to signed long long for easier row handling. */
    long long radius = (long long)r;
    long long pos = (long long)offset;

    /* Iterate over rows y = -radius … +radius.  For each row compute
     * its length and see if the desired cell lies in this row. */
    for (long long y = -radius; y <= radius; ++y)
    {
        long long abs_y      = y < 0 ? -y : y;
        long long row_len    = 1 + 2 * (radius - abs_y);      /* 2*(r-abs_y)+1 */
        if (pos < row_len)
        {
            long long x_start  = -(radius - abs_y);
            long long x        = x_start + pos;
            long long abs_x    = x < 0 ? -x : x;
            long long value    = radius - (abs_x + abs_y);
            return (seq_t)value;
        }
        pos -= row_len;
    }

    /* Should never reach here: guard for compiler warnings. */
    return (seq_t)0;
}
