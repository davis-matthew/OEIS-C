/*
 * A276254  With respect to the dictionary ordering of words over the alphabet
 *          {a,b}, i.e., e,a,b,aa,ab,ba,bb,aaa,..., the sequence is the
 *          characteristic function of the language of words that have no
 *          consecutive b's.
 * https://oeis.org/A276254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff the n-th word over {a,b} in dictionary order has no
 *            consecutive b's. Equivalently, let L = floor(log2(n+1)) and
 *            offset = n - (2^L - 1). Then a(n) = 1 iff offset has no
 *            consecutive 1-bits
 * Range:     u8 n <= 9595, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find L = floor(log2(n+1)) */
    idx_t t = n + 1;
    idx_t L = 0;
    while (t > 1) {
        t >>= 1;
        ++L;
    }

    /* Compute offset = n - (2^L - 1) */
    seq_t pow2L = (seq_t)1 << L;          /* 2^L   */
    seq_t offset = (seq_t)n - (pow2L - 1);

    /* Test for consecutive 1-bits */
    seq_t mask = offset & (offset >> 1);
    return mask ? (seq_t)0 : (seq_t)1;
}
