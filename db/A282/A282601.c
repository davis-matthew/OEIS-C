/*
 * A282601  a(n) = Sum_(k=1..phi(n)/2) floor(d_k/2) where d_k are the
 *          totatives of n.
 * https://oeis.org/A282601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..phi(n)/2} floor(d_k/2) where d_k are the
 *            totatives of n
 * Range:     u8 n <= 66, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"   /* provides static seq_t phi(idx_t m); */

/* Euclidean GCD for idx_t values. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1u) return (seq_t)0;  /* the sequence is defined for n >= 1 */

    /* Compute φ(n).  φ(1) comes out 1. */
    seq_t phi_val = phi(n);
    idx_t half_phi = (idx_t)(phi_val / 2u);

    seq_t sum = 0;
    idx_t seen = 0;

    for (idx_t k = 1; k <= n; ++k) {
        if (gcd_idx(k, n) == 1) {
            ++seen;
            if (seen <= half_phi) {
                sum += (seq_t)(k / 2u);   /* floor(k/2) */
            }
            if (seen >= half_phi) break;
        }
    }
    return sum;
}
