/*
 * A398861  Upper (3/2, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n*(9*n+1)/4) = (n*(9*n+1)+3)/4
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n values match the given initial terms exactly */
    if (n < 5) {
        static const seq_t init[5] = {0, 3, 10, 21, 37};
        return init[n];
    }

    /* Compute a(n) using the closed‑form formula */
    seq_t nn   = (seq_t)n;
    seq_t prod = nn * (nn * 9 + 1);   /* n*(9*n+1) */
    seq_t res  = (prod + 3) / 4;     /* (n*(9*n+1)+3)/4 truncates to floor */
    return res;
}
