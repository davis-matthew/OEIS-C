/*
 * A366430  Triprimes whose reversal is also a triprime.
 * https://oeis.org/A366430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 5117, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, bigomega, build_spf, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the n‑th positive integer that has exactly 3
   prime factors (with multiplicity) and whose decimal reversal also
   has exactly 3 prime factors. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/bigomega.h"
#include "../utils/append_candidate.h"
#include "../utils/rev.h"

/* Rebuild the cache up to `limit` and store it in `*cache_out` (new
   array).  The number of entries written is returned through `*len_out`. */
static void recompute_up_to(idx_t limit, seq_t **cache_out, size_t *len_out)
{
    /*--- Build smallest prime factor table ---*/
    idx_t *spf = NULL;
    build_spf(limit, &spf);
    if (!spf) {
        *len_out = 0;
        return;
    }

    /*--- Compute number of prime factors with multiplicity ---*/
    /* Allocate one extra for 0..limit inclusive */
    uint8_t *omega = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!omega) {
        free(spf);
        *len_out = 0;
        return;
    }
    omega[0] = omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        omega[i] = omega[i / spf[i]] + 1;

    /*--- Collect qualifying numbers ---*/
    seq_t *terms = NULL;
    size_t terms_cap = 0;
    size_t terms_len = 0;

    for (idx_t m = 2; m <= limit; ++m) {
        if (omega[m] != 3)
            continue;
        idx_t rev_m = rev(m);
        seq_t rev_omega = (rev_m <= limit) ? (seq_t)omega[rev_m] : bigomega(rev_m);
        if (rev_omega == 3)
            append_candidate(&terms, &terms_len, &terms_cap, (seq_t)m);
    }

    /*--- Clean up ---*/
    free(spf);
    free(omega);

    /*--- Replace old cache ---*/
    if (*cache_out)
        free(*cache_out);

    *cache_out = terms;
    *len_out   = terms_len;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for offset 0 */

    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static idx_t  current_lim = 0;

    /* initial bound sufficient for first several thousand terms */
    if (current_lim == 0) {
        current_lim = 200000;
        recompute_up_to(current_lim, &cache, &cache_len);
    }

    /* enlarge the precomputation until at least n terms are available. */
    while (n > cache_len) {
        current_lim <<= 1;                 /* double the bound */
        recompute_up_to(current_lim, &cache, &cache_len);
    }

    return cache[n - 1];  /* sequence is 1‑based */
}
