/*
 * A348674  Number of distinct values that can be produced by splitting n and
 *          adding the parts.
 * https://oeis.org/A348674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct sums obtainable by splitting the
 *            decimal representation of n into contiguous parts and adding the
 *            parts. Example: n = 120 → splits: 120, 12+0, 1+20, 1+2+0 → 4
 *            distinct sums
 * Range:     u8 n <= 9593, u16 n <= 9593, u32 n <= 9593, u64 n <= 9593,
 *            u128 n <= 9593, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compare_seq_t.h"

#define MAX_DIGITS 20                     /* because n <= 2^64-1 has ≤ 20 digits */
#define MAX_SPLITS (1U << (MAX_DIGITS-1)) /* 2^19 = 524288, enough for all cases */

seq_t a(idx_t n)
{
    /* --- convert n to its decimal digits -------------------------------- */
    int digits[MAX_DIGITS];
    int len = 0;
    if (n == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = (int)(tmp % 10);
            tmp /= 10;
        }
        /* reverse to get most‑significant digit first */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* --- enumerate all split patterns ------------------------------------ */
    static seq_t sums[MAX_SPLITS];
    size_t splits = (size_t)1 << (len - 1); /* 2^(len‑1) possible splits */
    for (size_t mask = 0; mask < splits; ++mask) {
        seq_t cur = 0;
        seq_t sum = 0;
        for (int i = 0; i < len; ++i) {
            cur = cur * 10 + (seq_t)digits[i];
            if (i == len - 1 || (mask & ((size_t)1 << i))) {
                sum += cur;
                cur = 0;
            }
        }
        sums[mask] = sum;
    }

    /* --- sort the sums and count unique values --------------------------- */
    if (splits > 1)
        qsort(sums, splits, sizeof(seq_t), compare_seq_t);

    uint64_t uniq = 1;          /* at least one sum exists (the sum of all parts) */
    for (size_t i = 1; i < splits; ++i)
        if (sums[i] != sums[i - 1])
            ++uniq;

    return (seq_t)uniq;
}
