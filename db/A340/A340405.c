/*
 * A340405  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(6,n) (with n at least 4) missing three edges, where the
 *          removed edges are incident to different vertices in the 6-point
 *          set and none of the removed edges are...
 * https://oeis.org/A340405
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 29791*63^(n-3) - 31050*31^(n-3) + 12369*15^(n-3) -
 *            2260*7^(n-3) + 19*3^(n-1) - 3 Comments: Implements the
 *            closed‑form given in the OEIS entry
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_small(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp != 0) {
        if (exp & 1U) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 4) return 0; /* sequence is defined for n ≥ 4 */
    idx_t e1 = n - 3;          /* exponent for 63,31,15,7 */
    idx_t e2 = n - 1;          /* exponent for 3 */

    seq_t term1 = 29791 * pow_seq_small((seq_t)63, e1);
    seq_t term2 = 31050 * pow_seq_small((seq_t)31, e1);
    seq_t term3 = 12369 * pow_seq_small((seq_t)15, e1);
    seq_t term4 = 2260  * pow_seq_small((seq_t) 7, e1);
    seq_t term5 = 19    * pow_seq_small((seq_t) 3, e2);

    /* use unsigned arithmetic; the final result is positive for n≥4 */
    seq_t sum1 = term1 + term3 + term5;
    seq_t sum2 = term2 + term4 + 3;

    return sum1 - sum2;
}
