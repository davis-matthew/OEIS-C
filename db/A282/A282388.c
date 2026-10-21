/*
 * A282388  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 462", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0; for n ≥ 1: a(n) = 3 × ∏{F_k | bit(k‑1) of n =
 *            1 and n ≥ 3·2^(k‑1)}, where F_k = 2^(2^k)+1 are the Fermat
 *            numbers (k ≥ 1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 95, u128 n <= 126,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)3;     /* the always–present factor 3 for n ≥ 1 */

    for (idx_t k = 1; ; ++k) {
        idx_t threshold = ((idx_t)3) << (k - 1);   /* 3·2^(k‑1) */
        if (threshold > n) {
            break;                      /* no higher k can satisfy thresholds */
        }

        if (n & (((idx_t)1) << (k - 1))) {          /* nth bit is set   */
            /* Fermat number F_k = 2^(2^k) + 1 */
            seq_t fermat = pow2((idx_t)1 << k);     /* 2^(2^k)          */
            fermat = fermat + (seq_t)1;             /* +1 to get F_k     */
            result = seq_mul(result, fermat);       /* multiply into result */
        }
    }

    return result;
}
