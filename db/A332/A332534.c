/*
 * A332534  Numbers that are not of the form prime + 2^(2^k) + m! with k >= 0,
 *          m >= 0.
 * https://oeis.org/A332534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer NOT expressible as prime + 2^(2^k) + m! for
 *            k>=0, m>=0
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seqarray, compute_complement
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seqarray.h"
#include "../utils/compute_complement.h"

/* Static cache variables */
static seq_t *comps = NULL;
static size_t comps_len = 0;
static idx_t current_limit = 0;

/* Ensure that comps contains at least n terms */
static void ensure_terms(idx_t n)
{
    if (n <= comps_len) return;

    /* Start with a reasonable limit and double until enough terms */
    idx_t lim = current_limit;
    if (lim == 0) lim = 1000000; /* initial guess */
    while (true) {
        compute_complement(lim, &comps, &comps_len, NULL);
        if (n <= comps_len) break;
        /* Not enough, double the limit and recompute */
        idx_t new_lim = lim * 2;
        free(comps); comps_len = 0;
        lim = new_lim;
    }
    current_limit = lim;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_terms(n);
    return comps[n - 1];
}
