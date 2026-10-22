/*
 * A345379  Number of terms m <= n, where m is a term in the bisection of
 *          Lucas numbers (A005248).
 * https://oeis.org/A345379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of Lucas numbers at even indices (L(2k)) that are
 *            <= n. (Sequence A345379)
 * Range:     u8 n <= 893, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t limit = (seq_t)n;           /* cast of n to seq_t for comparison */
    seq_t count = 0;

    /* Lucas numbers at even indices start with L(0)=2 */
    seq_t Lprevprev = (seq_t)2;       /* L(0)   */
    seq_t Lprev     = (seq_t)1;       /* L(1)   */

    /* Count L(0) if it does not exceed n (n >= 2 usually) */
    if (limit >= (seq_t)2)
        count = 1;

    /* Generate successive Lucas numbers;
       curr = L(idx+1) where idx starts at 1 */
    for (idx_t idx = 1; ; ++idx) {
        seq_t curr = Lprev + Lprevprev;   /* next Lucas number */

        /* Detect unsigned overflow (wrap‑around) */
        if (curr < Lprev)
            break;                        /* cannot represent further terms */

        /* Update for next iteration */
        Lprevprev = Lprev;
        Lprev     = curr;

        /* Stop once we pass the limit */
        if (curr > limit)
            break;

        /* curr corresponds to index idx+1.
           It is even iff idx is odd. */
        if ((idx & 1) == 1)
            ++count;
    }

    return count;
}
