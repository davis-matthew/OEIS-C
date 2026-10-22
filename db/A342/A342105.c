/*
 * A342105  Arithmetic numbers (A003601) that are not balanced numbers
 *          (A020492).
 * https://oeis.org/A342105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342105: Arithmetic numbers (A003601) that are not balanced
 * numbers (A020492).  A term m satisfies τ(m) | σ(m) and φ(m) ∤ σ(m). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/divisor_count_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* Static cache of previously found terms */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t cap = 0;               /* current capacity of cache */
    static size_t len = 0;               /* number of stored terms */
    static uint64_t last_checked = 1;    /* last integer examined */

    /* Ensure the caller's request can be satisfied */
    if (n <= len)
        return cache[n - 1];

    /* Grow the cache array lazily */
    if (cap == 0) {
        cap = 128;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Scan integers until the cache contains at least n terms */
    while (len < n) {
        ++last_checked;

        uint64_t tau = divisor_count_u64(last_checked);
        seq_t sigma = sigma_of(last_checked);
        seq_t phi_val = calc_phi((seq_t)last_checked);

        seq_t tau_seq = (seq_t)tau;

        /* Check the defining conditions */
        if ((sigma % tau_seq == 0) && (sigma % phi_val != 0)) {
            /* Store the term */
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
