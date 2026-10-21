/*
 * A241816  a(n) is the largest number smaller than n that can be obtained by
 *          swapping two adjacent bits in n, or n if no such number exists.
 * https://oeis.org/A241816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the largest number smaller than n that can be obtained *
 *            by swapping two adjacent bits in n by exchanging the first *
 *            "10" pair encountered from the least‑significant side. * If no
 *            such pair exists, a(n) = n
 * Range:     u8 n <= 256, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used by idx_t */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;                 /* cast index to value type */

    /* Handle zero immediately */
    if (N == 0)
        return N;

    /* Compute the number of significant bits in N (bitlen = floor(log2(N)) + 1) */
    unsigned bitlen = 0;
    seq_t tmp = N;
    while (tmp != 0) {
        tmp >>= 1;
        ++bitlen;
    }

    /* Scan from LSB upward to find the first "10" pattern */
    for (unsigned i = 1; i < bitlen; ++i) {
        if ( ((N >> i) & 1) && !((N >> (i - 1)) & 1) ) {
            /* Swap the two bits by toggling them */
            seq_t mask = ((seq_t)1 << (i - 1)) | ((seq_t)1 << i);
            return N ^ mask;
        }
    }

    /* No "10" pair was found; n is already maximal */
    return N;
}
