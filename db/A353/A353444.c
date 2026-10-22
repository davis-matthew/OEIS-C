/*
 * A353444  Integers m such that the decimal expansion of 1/m contains the
 *          digit 8.
 * https://oeis.org/A353444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A353444 – integers m such that 1/m has digit 8 in its decimal expansion.
   The function searches m in increasing order and returns the nth term (offset 1).
   It uses a timestamped visited‑remainder table to detect cycles without
   resetting the table for each m. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Visited remainder table (timestamped)                                */
/* --------------------------------------------------------------------- */
static uint64_t *vis = NULL;          /* vis[i] == current timestamp iff remainder i has been seen for current m */
static size_t   vis_cap = 0;          /* allocated capacity of vis (>= current maximum m) */
static uint64_t cur_ts = 1;           /* incremented for each new m */

/* Ensure the vis array can hold remainders 0 … m-1. */
static void ensure_vis_capacity(uint64_t m) {
    if (vis_cap < m) {
        size_t new_cap = m * 2;
        if (new_cap < m) new_cap = m + 1024;          /* avoid overflow for very small m */
        uint64_t *tmp = (uint64_t *)realloc(vis, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                            /* unrecoverable on allocation failure */
        vis = tmp;
        vis_cap = new_cap;
    }
}

/* Return true iff the decimal expansion of 1/m contains the digit 8.  */
static bool has_digit8_uint64(uint64_t m) {
    if (m == 0) return false;
    uint64_t rem = 1 % m;          /* first remainder after division by m */
    while (true) {
        if (rem == 0) break;       /* finite expansion: no more digits */
        if (vis[rem] == cur_ts) break; /* cycle detected: no 8 seen in this cycle */
        vis[rem] = cur_ts;          /* mark remainder as seen for this m */
        uint64_t digit = (rem * 10) / m;
        if (digit == 8) return true;
        rem = (rem * 10) % m;
    }
    return false;
}

/* --------------------------------------------------------------------- */
/*  Main sequence routine:  a(n) = the nth integer m with digit 8 in 1/m */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* safety for out‑of‑range index */
    seq_t result = 0;
    idx_t found = 0;
    for (uint64_t m = 1; ; ++m) {
        ensure_vis_capacity(m);
        if (has_digit8_uint64(m)) {
            ++found;
            if (found == n) {
                result = (seq_t)m;
                break;
            }
        }
        ++cur_ts;                              /* advance timestamp for next m */
    }
    return result;
}
