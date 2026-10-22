/*
 * A367993  Number of 4-cycles in the n X n white bishop graph.
 * https://oeis.org/A367993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)^2 * (2*n^2 - 4*n - 3 + 3*(-1)^n) / 24 for n ≥ 2
 *            (OEIS offset 2)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* For n < 2 we don't care; return 0 */
    if (n < 2) return 0;
    seq_t nSeq = (seq_t)n;
    seq_t nm1 = nSeq - 1;
    seq_t nm1_sq = nm1 * nm1;     /* (n-1)^2 */
    seq_t n2 = nSeq * nSeq;       /* n^2 */
    seq_t term = 2 * n2 - 4 * nSeq;  /* 2*n^2 - 4*n */
    /* Add 3*(-1)^n part: 0 if n even, -6 if n odd */
    if (n & 1) term -= (seq_t)6;   /* n is odd -> subtract 6 */
    /* Numerator */
    seq_t num = nm1_sq * term;
    /* Divide by 24 (exact division) */
    return num / 24;
}
