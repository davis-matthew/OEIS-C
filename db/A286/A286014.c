/*
 * A286014  Sum of smallest parts of all partitions of n into consecutive
 *          parts.
 * https://oeis.org/A286014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * compute a(n) for OEIS A286014:
 * sum of the smallest parts of all partitions of n into consecutive parts.
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* indices <1 are irrelevant */
    seq_t sum = 0;
    seq_t nseq = (seq_t)n;              /* convert once for speed */

    for (idx_t m = 1; ; ++m) {
        seq_t mseq = (seq_t)m;
        seq_t s = mseq * (mseq + 1) / 2;      /* s = m*(m+1)/2 */
        if (s > nseq) break;                 /* no larger m possible */
        seq_t rem = nseq - mseq * (mseq - 1) / 2;
        if (rem % mseq == 0) {               /* k integer */
            sum += rem / mseq;               /* add smallest part k */
        }
    }
    return sum;
}
