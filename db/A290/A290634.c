/*
 * A290634  Positive integers which are never the quotient of phi(n)/tau(n).
 * https://oeis.org/A290634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that never equals φ(m)/τ(m) for
 *            any m. The implementation precomputes φ(k) and τ(k) up to a
 *            dynamically expanded limit, records all attainable quotients,
 *            and collects the missing integers. The cached sequence is
 *            rebuilt each time the requested n exceeds the curren...
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   compute_missing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/compute_missing.h"

/* Global cache of the sequence values */
static seq_t *cached_seq = NULL;      /* array of missing quotients */
static idx_t  cached_len = 0;         /* number of stored values   */
static idx_t  current_limit = 0;      /* sieve limit used last run */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public function: returns the n-th term of A290634 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;                /* below defined range */

    /* If the cache does not yet contain enough terms, rebuild with a larger limit. */
    while (cached_len < n) {
        /* Choose a reasonable sieve limit: start at 1 000 000,
           then double until we have enough terms. */
        idx_t new_limit = current_limit ? current_limit * 2 : 1000000ULL;
        seq_t *new_seq = NULL;
        idx_t new_len = 0;
        compute_missing(new_limit, &new_seq, &new_len);

        /* Replace the current cache */
        if (cached_seq) free(cached_seq);
        cached_seq = new_seq;
        cached_len = new_len;
        current_limit = new_limit;
    }

    return cached_seq[n - 1];
}
