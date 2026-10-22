/*
 * A355858  a(n) = n^(2*n-1) mod (2*n-1).
 * https://oeis.org/A355858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(2*n-1) mod (2*n-1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* 2*n-1 fits in idx_t for the intended ranges */
    idx_t mod_idx = 2 * n - 1;
    seq_t mod = (seq_t)mod_idx;
    seq_t base = (seq_t)n;

    /* when mod == 1 the result is always 0 */
    if (mod == 1) return 0;

    uint64_t exp = 2 * n - 1;
    return mod_pow(base, exp, mod);
}
