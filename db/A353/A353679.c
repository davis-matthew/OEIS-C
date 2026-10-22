/*
 * A353679  Odd numbers k for which phi(k) = phi(sigma(k)).
 * https://oeis.org/A353679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* AEIS A353679: odd numbers k with φ(k) = φ(σ(k)).
 * The function a(n) returns the nth such number (1‑based) by
 * incremental search and memoisation.  */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"   /* seq_t calc_phi(seq_t m); */
#include "../utils/sigma_of.h"  /* seq_t sigma_of(idx_t n); */

/* Array holding the terms that have been found so far. */
static seq_t *found = NULL;
static size_t found_cnt = 0;   /* number of stored terms   */
static size_t found_cap = 0;   /* current capacity        */
static uint64_t last_checked = 0;   /* last odd number examined */

/* Make sure 'found' has capacity at least 'need'. */
static void
ensure_found_capacity(size_t need)
{
    if (need <= found_cap) return;
    size_t newcap = found_cap ? found_cap * 2 : 8;
    if (newcap < need) newcap = need;
    seq_t *tmp = (seq_t *)realloc(found, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);      /* allocation failure */
    found = tmp;
    found_cap = newcap;
}

/* Append a newly found term to the global array. */
static void
add_found(seq_t val)
{
    ensure_found_capacity(found_cnt + 1);
    found[found_cnt++] = val;
}

/* Return the nth term, computing new terms as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, but never requested */

    while (found_cnt < n) {
        ++last_checked;
        uint64_t k = last_checked;
        if ((k & 1ULL) == 0) continue;              /* only odd k are relevant */

        seq_t phi_k     = calc_phi((seq_t)k);
        seq_t sigma_k   = sigma_of(k);
        seq_t phi_sigma = calc_phi(sigma_k);

        if (phi_k == phi_sigma) {
            add_found((seq_t)k);
        }
    }
    return found[n - 1];
}
