/*
 * A397269  a(n) = n*(8*n^2-9*n-2-(9*n-6)*(-1)^n)/48.
 * https://oeis.org/A397269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A397269: a(n) = n*(8*n^2-9*n-2-(9*n-6)*(-1)^n)/48
   Equivalent parity formulas used for efficient computation:
      a(2*k)   = k*(k-1)*(8*k-1)/6
      a(2*k+1) = 2*k*(k+1)*(2*k+1)/3
   The function works for all n >= 0 and uses only integer arithmetic. */

seq_t a(idx_t n)
{
    /* Even index case: a(2*k) = k*(k-1)*(8*k-1)/6 */
    if ((n & 1) == 0) {
        seq_t k = (seq_t)(n >> 1);   /* k = n/2 */
        if (k <= 1)                 /* k = 0 or 1 gives 0 */
            return 0;

        seq_t term1 = k * (k - 1);          /* k*(k-1) */
        seq_t term2 = 8 * k - 1;            /* 8*k-1     */
        seq_t prod  = term1 * term2;        /* (k*(k-1))*(8*k-1) */
        return prod / 6;                    /* exact division */
    }

    /* Odd index case: a(2*k+1) = 2*k*(k+1)*(2*k+1)/3 */
    seq_t k = (seq_t)((n - 1) >> 1);   /* k = (n-1)/2 */
    if (k == 0)                        /* k = 0 gives 0 */
        return 0;

    seq_t term1 = k * (k + 1);          /* k*(k+1)   */
    seq_t term2 = 2 * k + 1;            /* 2*k+1     */
    seq_t prod  = term1 * term2;        /* (k*(k+1))*(2*k+1) */
    prod = prod * 2;                    /* multiply by 2 */
    return prod / 3;                    /* exact division */
}
