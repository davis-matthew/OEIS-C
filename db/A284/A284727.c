/*
 * A284727  Dimensions of certain centralizer algebras associated with
 *          Bratelli diagram B(S_6, S_5).
 * https://oeis.org/A284727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Σ_{k=0}^{6} S(n,k) where S(n,k) is the Stirling number of
 *            the second kind. This counts the set partitions of an n-element
 *            set into at most 6 blocks
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Stirling numbers S(i,k) for k = 0..6.
       Initialize for i = 0: S(0,0) = 1, others 0. */
    seq_t s0 = 1, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Update from high k to low to use old values. */
        seq_t ns6 = 6 * s6 + s5;
        seq_t ns5 = 5 * s5 + s4;
        seq_t ns4 = 4 * s4 + s3;
        seq_t ns3 = 3 * s3 + s2;
        seq_t ns2 = 2 * s2 + s1;
        seq_t ns1 =       s1 + s0;  /* 1*s1 + s0 */

        s0 = 0;  /* S(i,0) = 0 for i >= 1 */
        s1 = ns1;
        s2 = ns2;
        s3 = ns3;
        s4 = ns4;
        s5 = ns5;
        s6 = ns6;
    }

    /* Sum S(n,0..6).  For n>=1, s0 = 0, but we include it for n=0. */
    seq_t total = s0 + s1 + s2 + s3 + s4 + s5 + s6;
    return total;
}
