/*
 * A342469  Positive numbers that are triangular, square or pentagonal.
 * https://oeis.org/A342469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that is triangular, square or
 *            pentagonal
 * Range:     u8 n <= 43, u16 n <= 816, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term (1‑based) of A342469.
   Uses a simple incremental merge of the three increasing sequences
   T(k)=k(k+1)/2,  S(k)=k*k,  P(k)=k(3k-1)/2.  No memoisation or dynamic
   allocation is required; each call recomputes the first n terms in
   O(n).  Works for any seq_t that supports +,-,*,/,<,== and the usual
   integer arithmetic. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe  */
    idx_t t = 1, s = 1, p = 1;            /* counters for the sequences   */
    seq_t cur = 0;                        /* current value                 */

    for (idx_t count = 1; count <= n; ++count) {
        /* Compute next candidates from each sequence */
        seq_t t_val = (seq_t)t * ((seq_t)t + 1) / 2;
        seq_t s_val = (seq_t)s * (seq_t)s;
        seq_t p_val = (seq_t)p * ((seq_t)3 * (seq_t)p - 1) / 2;

        /* Take the smallest candidate */
        cur = t_val;
        if (s_val < cur) cur = s_val;
        if (p_val < cur) cur = p_val;

        /* Advance all sequences that produced the current value */
        if (t_val == cur) ++t;
        if (s_val == cur) ++s;
        if (p_val == cur) ++p;
    }
    return cur;
}
