/*
 * A278754  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 65", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(k+1)-1)/9 * 10^k for n = 2k (even) (k ≥ 1) * a(n) =
 *            (10^k-1)/9 for n = 2k+1 (odd) (k ≥ 1) * with a(0) = 1, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* first two terms are special */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    /* even index: n = 2k */
    if ((n & (idx_t)1) == 0) {
        idx_t k = n >> 1;                 /* k = n/2  */
        seq_t ten_k_plus_1 = pow10(k + 1); /* 10^(k+1) */
        seq_t ones = (ten_k_plus_1 - (seq_t)1) / (seq_t)9; /* k+1 ones */
        seq_t result = ones * pow10(k);   /* append k zeros */
        return result;
    }

    /* odd index: n = 2k+1 */
    idx_t k = n >> 1;                     /* k = floor(n/2) */
    /* k == 0 corresponds to n == 1, already handled above */
    seq_t ten_k = pow10(k);               /* 10^k */
    seq_t result = (ten_k - (seq_t)1) / (seq_t)9; /* k ones */
    return result;
}
