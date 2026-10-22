/*
 * A305734  Integers m such that m, m^2 and m^3 are all distinct-digit
 *          numbers.
 * https://oeis.org/A305734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   digit_distinct_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A305734: integers m (0 ≤ m ≤ 2154) with m, m² and m³
   all having distinct decimal digits. */
#include <stdbool.h>
#include <stddef.h>         /* for size_t */
#include "../utils/digit_distinct_idx.h"

/* Storage for the sequence values (2155 possible entries). */
static seq_t seq_vals[2155];
static idx_t seq_len = 0;
static bool seq_ready = false;

/* Compute the sequence once and cache the results. */
static void compute_sequence(void) {
    idx_t m;
    for (m = 0; m <= 2154; ++m) {
        if (digit_distinct_idx(m) &&
            digit_distinct_idx(m * m) &&
            digit_distinct_idx(m * m * m)) {
            seq_vals[seq_len++] = (seq_t)m;
        }
    }
    seq_ready = true;
}

/* Public function matching the required signature. */
seq_t a(idx_t n) {
    if (!seq_ready) compute_sequence();
    if (n == 0) return 0;                /* offset is 1; undefined for n==0 */
    if (n <= seq_len) return seq_vals[n - 1];
    return 0;                            /* undefined for n beyond the sequence length */
}
