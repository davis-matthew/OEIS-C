/*
 * A262620  Number of "ON" cells at n-th stage in simple 2-dimensional
 *          cellular automaton on the square grid (see Comments lines for
 *          definition).
 * https://oeis.org/A262620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 + 4 * A261692(n) where A261692(n) = Σ_{i=1..n} d_i and
 *            d_i = 2^(⌊log₂i⌋+1) - 1 - 2*(i - 2^⌊log₂i⌋) A compact closed
 *            form is A261692(n) = Σ_{k=0}^{K-1} 2^(2k) + m*(2^(K+1)-m) with K
 *            = ⌊log₂n⌋ and m = n - 2^K + 1
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute K = floor(log2 n) and twoPowerK = 2^K */
    idx_t K = 0;
    idx_t twoPowerK = 1;
    /* Shift until the next power would exceed n */
    while ((idx_t)twoPowerK << 1 <= n) {
        twoPowerK <<= 1;
        ++K;
    }

    /* Length of the partial block */
    idx_t m = n - twoPowerK + 1;

    /* Sum of full blocks: Σ_{k=0}^{K-1} 2^(2k) = (4^K - 1)/3 */
    seq_t sum_full_blocks = (seq_pow((seq_t)4, K) - (seq_t)1) / (seq_t)3;

    /* Sum over the partial block */
    seq_t twoPowerK1 = ((seq_t)twoPowerK) << 1;          /* 2^(K+1) */
    seq_t partial = (seq_t)m * (twoPowerK1 - (seq_t)m);

    /* Compute the answer */
    seq_t result = sum_full_blocks + partial;
    result = result * ((seq_t)4) + ((seq_t)1);

    return result;
}
