/*
 * A256249  Partial sums of A006257 (Josephus problem).
 * https://oeis.org/A256249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum_{i=1..n} (2*(i - 2^floor(log2 i)) + 1)
 * Range:     u8 n <= 15, u16 n <= 463, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   which simplifies to:                                 */
/*   a(n) = n*(n+2) - ( 2*(4^floor(log2 n)-1)/3 + 2^{floor(log2 n)+1}*(n-2^floor(log2 n)+1) ) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find floor(log2(n)) */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++L;

    /* Powers of 2 and 4 */
    seq_t pow4   = ((seq_t)1) << (2 * L);        /* 4^L = 2^{2L} */
    seq_t pow2L  = ((seq_t)1) << L;              /* 2^L          */
    seq_t pow2L1 = ((seq_t)1) << (L + 1);        /* 2^{L+1}      */

    /* Compute the sum of the power terms */
    seq_t P = ((seq_t)2 * (pow4 - (seq_t)1)) / (seq_t)3;
    P += pow2L1 * ((((seq_t)n) - pow2L) + (seq_t)1);

    /* Final expression */
    seq_t res = ((seq_t)n) * ((((seq_t)n) + (seq_t)2)) - P;
    return res;
}
