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
 * Helpers:   build_phi_and_totients
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) = start of each run of exactly three consecutive even totient numbers.
   The sequence starts with A394637: 28, 136, 176, ...  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_phi_and_totients.h"

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
        starts = (seq_t*)malloc(start_cap * sizeof(seq_t));
    } else if (start_cnt >= start_cap) {
        start_cap <<= 1;
        starts = (seq_t*)realloc(starts, start_cap * sizeof(seq_t));
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
