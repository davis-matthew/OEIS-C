/*
 * A392706  Least even-indexed part in row n of A391983 (the 0-based partial
 *          alternating sums of the n-th composition in standard order).
 * https://oeis.org/A392706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, changed
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 8936 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A392706: the least part at an even (0-based) position of row n of A391983:
 * the partial alternating sums q_1, q_2 - q_1, q_3 - q_2 + q_1, ... of the n-th
 * composition q in standard order (the gaps between the 1 bits of n, read
 * from the top). Computed directly from the bits of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t x = (uint64_t)n;
    int pos[64], np = 0;                      /* 1-based positions of the 1 bits, low first */
    for (int i = 0; i < 64; ++i) if ((x >> i) & 1) pos[np++] = i + 1;
    /* parts, most significant first: differences of 0, pos[0], pos[1], ... reversed */
    int64_t s = 0, best = INT64_MAX;
    for (int j = 0; j < np; ++j) {
        int k = np - 1 - j;                   /* part j comes from the gap below pos[k] */
        int64_t q = pos[k] - (k ? pos[k - 1] : 0);
        s = q - s;                            /* pas_j = q_j - pas_(j-1) */
        if (!(j & 1) && s < best) best = s;
    }
    return (seq_t)best;
}
