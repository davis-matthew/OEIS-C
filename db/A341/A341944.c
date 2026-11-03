/*
 * A341944  Next larger integer with same number of runs in binary expansion
 *          as n.
 * https://oeis.org/A341944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest integer > n whose binary representation has
 *            the same number of runs (consecutive equal bits) as n. This
 *            implements the definition of OEIS A341944
 * Range:     u8 n <= 169, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 82 terms through n = 7658 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static size_t count_runs_idx(idx_t x) {
    size_t runs = 0;
    int prev = -1;                    /* sentinel for first bit */
    while (x != 0) {
        int bit = (int)(x & (idx_t)1);
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        x >>= 1;
    }
    return runs;
}

static size_t count_runs_seq(seq_t x) {
    size_t runs = 0;
    int prev = -1;
    while (x != (seq_t)0) {
        seq_t bit = x & (seq_t)1;
        int bit_int = (int)(bit);
        if (bit_int != prev) {
            ++runs;
            prev = bit_int;
        }
        x >>= 1;
    }
    return runs;
}

seq_t a(idx_t n) {
    if (n == 0)          /* safety for out‑of‑range indices */
        return 1;

    size_t target = count_runs_idx(n);
    seq_t y = (seq_t)n + (seq_t)1;            /* candidate > n */

    while (count_runs_seq(y) != target)
        ++y;

    return y;
}
