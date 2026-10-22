/*
 * A333820  a(n) is the number of pairs (q,r) such that q <= r and phi(q) +
 *          phi(r) = 2*n.
 * https://oeis.org/A333820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (q,r) with q <= r and φ(q)+φ(r)=2*n
 * Range:     u8 none, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   compute_phi_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_phi_upto.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1, but guard just in case */

    idx_t target = 2 * n;            /* φ(q)+φ(r) = 2*n */
    idx_t limit  = target * 4 + 1000; /* safe upper bound for q,r */

    /* Allocate and compute φ values */
    seq_t *phi = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!phi) return 0;
    compute_phi_upto(limit, phi);

    /* Frequency array for φ values seen so far (up to target) */
    seq_t *freq = (seq_t *)calloc((size_t)(target + 1), sizeof(seq_t));
    if (!freq) {
        free(phi);
        return 0;
    }

    seq_t ans = 0;
    for (idx_t q = limit; q >= 1; --q) {
        seq_t phi_q = phi[q];
        if (phi_q <= target) {
            /* Add current q as a potential r */
            freq[(idx_t)phi_q]++;

            /* Count r with φ(r) = target - φ(q) */
            seq_t need = target - phi_q;   /* need in [0, target] */
            ans += freq[(idx_t)need];
        }
    }

    free(phi);
    free(freq);
    return ans;
}
