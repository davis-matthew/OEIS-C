/*
 * A260466  Number of integers in Pascal's triangle strictly between 1 and n.
 * https://oeis.org/A260466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of binomial coefficients C(r,k) strictly between 1 and n
   for all r ≥ 2 and 1 ≤ k ≤ r − 1.  The implementation enumerates all
   interior binomial coefficients up to the point where they reach or exceed n
   and counts those that are < n.  This satisfies the definition of A260466
   with a time complexity of O(n^2) worst case (≈ n²/2 elementary
   operations for n ≤ 10 000). */
seq_t a(idx_t n)
{
    seq_t cnt = 0;          /* result */
    /* r ranges from 2 to n-1 inclusive; the loop stops automatically
       for n ≤ 2 because r starts at 2 and the condition r < n fails. */
    for (idx_t r = 2; r < n; ++r) {
        /* C(r,1) = r ; always > 1 */
        seq_t val = (seq_t)r;
        if (val < (seq_t)n) {
            /* count the mirror of C(r, r-1) if distinct */
            if (r == 2) {
                /* symmetric when r = 2 (k = r/2) */
                cnt += 1;
            } else {
                cnt += 2;
            }
        }

        /* Now iterate k from 2 up to floor(r/2).  For each step we
           compute the next binomial coefficient using the recurrence
           C(r,k) = C(r,k-1) * (r-k+1) / k.  Once val >= n we can
           stop because further values will only increase. */
        for (idx_t k = 2; k <= r / 2; ++k) {
            /* multiply then divide – division is exact by binomial
               coefficient properties. */
            val = (val * (seq_t)(r - k + 1)) / (seq_t)k;
            if (val >= (seq_t)n) {
                break;
            }
            /* add 1 for the central binomial coefficient when r is even
               and 2 for the pair of symmetric coefficients otherwise. */
            cnt += (k == r - k) ? 1 : 2;
        }
    }
    return cnt;
}
