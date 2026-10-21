/*
 * A295926  Number of (not necessarily maximal) cliques in the
 *          n-cube-connected cycle graph.
 * https://oeis.org/A295926
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*2^(n-1)*n + 1 for n > 3. The offset is 3, so a(3) = 69
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Undefined or trivial values for n < 3 are not required by OEIS, but we return 0. */
    if (n < 3) return (seq_t)0;

    /* Exact value for the offset term. */
    if (n == 3) return (seq_t)69;

    /* For n > 3 use the closed‑form formula. */
    seq_t pow = pow2(n - 1);       /* 2^(n-1) */
    seq_t res = pow * (seq_t)n;    /* * n */
    res = res * (seq_t)5;          /* * 5 */
    res = res + (seq_t)1;          /* + 1 */
    return res;
}
