/*
 * A270127  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 86", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n} c(k) c(0) = 1 c(1) = 5 c(2m) = 2^{⌊log₂(m)⌋ +
 *            popcnt(m) + 2} (m ≥ 1) c(2m+1) = 3 · c(2m) (m ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 75, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   _popcount_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_popcount_idx.h"
/* Helper: floor of log₂ for non‑zero idx_t */
static inline idx_t _floor_log2_idx(idx_t x) {
    idx_t r = 0;
    while (x > 1) {
        x >>= 1;
        ++r;
    }
    return r;
}

/* Number of active cells in the n‑th stage */
static seq_t _stage_cells(idx_t n) {
    if (n == 0) return (seq_t)1;
    if ((n & 1ULL) == 0) {                 /* even n = 2m */
        idx_t m = n >> 1;
        idx_t log = _floor_log2_idx(m);
        int pc = _popcount_idx(m);
        seq_t exp = (seq_t)(log + pc + 2);
        return ((seq_t)1) << (unsigned int)exp;
    } else {                                /* odd n */
        if (n == 1) return (seq_t)5;
        idx_t m = n - 1;                    /* even predecessor */
        idx_t k = m >> 1;                   /* k = m/2 = (n-1)/2 */
        idx_t log = _floor_log2_idx(k);
        int pc = _popcount_idx(k);
        seq_t exp = (seq_t)(log + pc + 2);
        seq_t val = ((seq_t)1) << (unsigned int)exp;
        return (seq_t)3 * val;
    }
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        sum += _stage_cells(i);
    }
    return sum;
}
