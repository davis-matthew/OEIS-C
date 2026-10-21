/*
 * A263318  Number of aperiodic necklaces (Lyndon words) with 9 black beads
 *          and n white beads.
 * https://oeis.org/A263318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(n+9, 9) - (n+9)%3==0 ? binomial((n+9)/3, 3) :
 *            0) / (n+9)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 33, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS definition A263318 for all n >= 0.  */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* total = n + 9 */
    idx_t total = n + (idx_t)9;
    /* main binomial term */
    seq_t res = binomial(total, 9);
    /* subtract the correction if n+9 divisible by 3 */
    if ((total % 3) == 0) {
        res -= binomial(total / 3, 3);
    }
    /* divide by (n+9) exactly */
    return res / (seq_t)total;
}
