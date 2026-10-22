/*
 * A338804  A sequence containing each nonnegative integer exactly twice, such
 *          that for all k, k numbers appear in the sequence between the first
 *          and second appearances of k.
 * https://oeis.org/A338804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is obtained by concatenating Langford
 * sequences of order 3^k (k ≥ 1).  For n = 1, 2 we return 0.
 * For n ≥ 3 let k = ⌊log_3 n⌋, N = 3^k.  The block [3^k, 3^{k+1}−1]
 * contains 2N terms and uses the numbers { (3^k−1)/2, … , (3^k−1)/2 + N − 1 }.
 * The following formula reproduces that block in O(1):
 *
 *   Let m = (N − 1) / 2  (integer division).
 *
 *   if offset < N  (first half of the block)
 *       if offset < m
 *           value = N + offset
 *       else
 *           value = offset
 *   else          (second half)
 *       pos2 = offset − N
 *       if pos2 is even
 *           r = pos2 / 2
 *           value = m + r
 *       else
 *           r = pos2 / 2
 *           value = N + r
 *
 * The value returned is a(n).  No additional memory is used and
 * the computation is O(log n) for the power‑of‑three search.  */

seq_t a(idx_t n)
{
    /* n ≤ 2  ⇒  the two initial zeros */
    if (n <= 2)
        return (seq_t)0;

    /* Find the largest power of 3 that does not exceed n */
    idx_t pow3 = 1;               /* 3^0   */
    while (1) {
        unsigned __int128 next = (unsigned __int128)pow3 * 3;
        if (next > n)            /* next would exceed n */
            break;
        pow3 = (idx_t)next;      /* pow3 = 3^k with 3^k ≤ n < 3^{k+1} */
    }

    /* N is the size of the current block */
    idx_t N = pow3;                /* N = 3^k  (k ≥ 1) */
    idx_t offset = n - N;           /* 0‑based position inside the block */

    /* m = (N-1)/2 ensures integer division uses only integer arithmetic */
    idx_t m = (N - 1) >> 1;          /* (N-1)/2 */

    seq_t value;
    if (offset < N) {                       /* first half of the block */
        if (offset < m)                     /* elements N … N+m-1  */
            value = (seq_t)(N + offset);
        else                                 /* elements m … N-1    */
            value = (seq_t)offset;
    } else {                                 /* second half of the block */
        idx_t pos2 = offset - N;              /* index inside the second half */
        if ((pos2 & 1) == 0) {                /* even indices – H2 sequence */
            idx_t r = pos2 >> 1;              /* r = 0 … m   */
            value = (seq_t)(m + r);
        } else {                              /* odd indices – H1 sequence */
            idx_t r = pos2 >> 1;              /* r = 0 … m-1 */
            value = (seq_t)(N + r);
        }
    }
    return value;
}
