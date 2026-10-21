/*
 * A243569  Unsigned Stirling numbers of the first kind s(n,8).
 * https://oeis.org/A243569
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = s(n,8), the unsigned Stirling number of the first kind
 *            for k = 8, computed by the recurrence s(0,0) = 1, s(n,0) = 0
 *            (n>0) s(n,k) = s(n-1,k-1) + (n-1) * s(n-1,k) (n>0, k>0)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation stores two rows of the recursion table in fixed
   arrays of length 9 (0 … 8). */

#define K 8

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 8; return 0 otherwise. */
    if (n < 8) return 0;

    seq_t prev[K + 1] = {0};
    seq_t cur[K + 1];
    prev[0] = 1;                    /* s(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = 0;
        seq_t mult = (seq_t)(i - 1);   /* (i-1) as seq_t */
        for (int k = 1; k <= K; ++k) {
            cur[k] = prev[k - 1] + prev[k] * mult;
        }
        /* copy current row into previous row for next iteration */
        for (int k = 0; k <= K; ++k) {
            prev[k] = cur[k];
        }
    }

    return prev[K];                  /* s(n,8) */
}
