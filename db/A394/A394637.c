/*
 * A394637  Start of exactly three even numbers that are totient numbers
 *          (A002202).
 * https://oeis.org/A394637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = start of each run of exactly three consecutive even totient numbers.
   The sequence starts with A394637: 28, 136, 176, ...  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: build the totient sieve up to 'lim' and mark totients in is_t. */
static void build_phi_and_totients(idx_t lim, uint32_t **phi_out, unsigned char **tot_out)
{
    uint32_t *phi = (uint32_t*)realloc(*phi_out, (size_t)(lim + 1) * sizeof(uint32_t));
    unsigned char *tot = (unsigned char*)realloc(*tot_out, (size_t)(lim + 1) * sizeof(unsigned char));

    for (idx_t i = 0; i <= lim; ++i) {
        phi[i] = (uint32_t)i;
        tot[i] = 0;
    }

    for (idx_t i = 2; i <= lim; ++i) {
        if (phi[i] == i) {               /* i is prime */
            uint32_t p = (uint32_t)i;
            for (idx_t j = i; j <= lim; j += i) {
                phi[j] = (uint32_t)((uint64_t)phi[j] / p * (p - 1));
            }
        }
    }

    for (idx_t k = 1; k <= lim; ++k) {
        uint32_t v = phi[k];
        if (v <= lim) tot[v] = 1;
    }

    *phi_out = phi;
    *tot_out = tot;
}

/* Static data shared between calls. */
static idx_t      current_lim = 0;
static uint32_t  *phi_table  = NULL;
static unsigned char *totient = NULL;
static seq_t    *starts     = NULL;
static size_t   start_cap   = 0;
static idx_t    start_cnt   = 0;
static idx_t    scan_pos    = 2;            /* next even number to examine */

/* Expand the totient sieve until it covers 'needed'. */
static void ensure_limit(idx_t needed)
{
    if (current_lim >= needed) return;
    idx_t new_lim = current_lim ? (current_lim << 1) : 100000;
    while (new_lim < needed) new_lim <<= 1;
    build_phi_and_totients(new_lim, &phi_table, &totient);
    current_lim = new_lim;
}

/* Make room in the array storing found starts. */
static void grow_starts(void)
{
    if (start_cap == 0) {
        start_cap = 64;
        starts = mem_new(seq_t, start_cap);
    } else if (start_cnt >= start_cap) {
        start_cap <<= 1;
        starts = mem_resize(seq_t, starts, start_cap);
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but guard */
    while (start_cnt < n) {
        /* Need to be able to test m, m+2, m+4, (m+6) for totients. */
        ensure_limit(scan_pos + 6);

        /* Scan even numbers until we either find the nth start or run out of limit. */
        for ( ; scan_pos + 6 <= current_lim && start_cnt < n; scan_pos += 2) {
            if (!totient[scan_pos]) continue;
            if (!totient[scan_pos + 2]) continue;
            if (!totient[scan_pos + 4]) continue;
            if (totient[scan_pos + 6]) continue;          /* must end exactly after 3 */
            bool left_ok = (scan_pos > 2) ? !totient[scan_pos - 2] : true;
            if (!left_ok) continue;

            grow_starts();
            starts[start_cnt++] = (seq_t)scan_pos;
        }
    }
    return starts[n - 1];
}
