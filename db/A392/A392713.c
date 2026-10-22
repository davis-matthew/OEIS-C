/*
 * A392713  Least part after the leading zero in row n of A391983 (the 0-based
 *          partial alternating sums of the n-th composition in standard
 *          order).
 * https://oeis.org/A392713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = min_{j≥1} s_j * where s_0 = 0 and s_j = -s_{j-1} + part_j
 *            * and the composition parts are obtained from n by * taking the
 *            positions of 1-bits in the reversed binary * expansion of n,
 *            prepending 0, taking first differences, * and reversing. The
 *            first part (after the leading 0) * corresponds...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based index of the composition. */
    idx_t k = n;                 /* copy to avoid modifying argument */
    idx_t pos[65];               /* positions of 1‑bits in reversed binary */
    idx_t pos_cnt = 1;           /* pos[0] = 0 */
    pos[0] = 0;
    idx_t bit_pos = 1;           /* bit position (1‑based) in reversed order */

    while (k > 0) {
        if (k & 1ULL)            /* bit is 1 */
            pos[pos_cnt++] = bit_pos;
        k >>= 1;
        ++bit_pos;
    }

    /* Compute partial alternating sums, keep the minimum (excluding s_0) */
    seq_t val = (seq_t)(pos[pos_cnt - 1] - pos[pos_cnt - 2]);   /* s_1 */
    seq_t min_val = val;

    for (idx_t i = pos_cnt - 2; i >= 1; --i) {
        seq_t diff = (seq_t)(pos[i] - pos[i - 1]);
        val = -val + diff;
        if (val < min_val)
            min_val = val;
    }

    return min_val;
}
