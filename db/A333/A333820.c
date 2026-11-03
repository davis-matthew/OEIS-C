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
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Build the totient table up to 'limit' (inclusive). */
static void
compute_phi_upto(idx_t limit, seq_t *phi)
{
    for (idx_t i = 0; i <= limit; ++i) phi[i] = i;
    for (idx_t p = 2; p <= limit; ++p) {
        if (phi[p] == p) {                  /* p is prime */
            for (idx_t k = p; k <= limit; k += p)
                phi[k] -= phi[k] / p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1, but guard just in case */

    idx_t target = 2 * n;            /* φ(q)+φ(r) = 2*n */
    idx_t limit  = target * 4 + 1000; /* safe upper bound for q,r */

    /* Allocate and compute φ values */
    seq_t *phi = mem_new(seq_t, (size_t)(limit + 1));
    if (!phi) return 0;
    compute_phi_upto(limit, phi);

    /* Frequency array for φ values seen so far (up to target) */
    seq_t *freq = mem_new(seq_t, (size_t)(target + 1));
    if (!freq) {
        mem_free(phi);
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

    mem_free(phi);
    mem_free(freq);
    return ans;
}
