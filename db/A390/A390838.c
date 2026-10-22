/*
 * A390838  Irregular table, where row n holds the lower left triangle of the
 *          reduced multiplication table modulo n (without row/column 0), read
 *          by rows.
 * https://oeis.org/A390838
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value of the (n‑1)th entry in the flattened lower‑left
 *            triangle of the reduced multiplication table modulo row r, where
 *            the row number r is the smallest integer with S(r) ≥ n‑1 and
 *            S(r) = r(r‑1)(r+1)/6. The entry at position (i,j) in row r is
 *            (i·j) mod r, with 1 ≤ i < r, 1 ≤ j ≤ i. The cod...
 * Range:     u8 n <= 121, u16 n <= 121, u32 n <= 121, u64 n <= 121,
 *            u128 n <= 121, big any n
 * Helpers:   S128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/S128.h"

/* Triangular numbers: T(i) = i(i+1)/2 */
static inline idx_t triangular(idx_t i)
{
    return i * (i + 1) / 2;
}

seq_t a(idx_t n)
{
    /* the OEIS offset is 2, so a(2) corresponds to the first term
       of the flattened sequence (global index 1).  Use k = n‑1. */
    if (n < 2) return 0;              /* undefined for n<2, but guard against */
    idx_t k = n - 1;                  /* global position (1‑based) */

    /* --- find minimal row r with S(r) ≥ k --- */
    idx_t low = 2, high = 2;
    /* increase high until it covers k */
    while (S128(high) < (__int128)k) {
        if (high > (idx_t)(~(idx_t)0) / 2) {  /* avoid overflow */
            high = (idx_t)(~(idx_t)0);
            break;
        }
        high <<= 1;
    }
    /* binary search between low and high */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (S128(mid) >= (__int128)k)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */

    /* --- position within the row --- */
    idx_t S_r_minus1 = (idx_t)S128(r - 1);   /* safe because r‑1 < r ≤ 2^64 */
    idx_t pos = k - S_r_minus1;              /* 1‑based position in row r */

    /* --- find i such that triangular(i) ≥ pos --- */
    idx_t i_low = 1, i_high = r - 1;
    while (i_low < i_high) {
        idx_t mid = i_low + (i_high - i_low) / 2;
        if (triangular(mid) >= pos)
            i_high = mid;
        else
            i_low = mid + 1;
    }
    idx_t i = i_low;
    idx_t tri_i_minus1 = triangular(i - 1);    /* i≥1 */
    idx_t j = pos - tri_i_minus1;              /* 1‑based j, 1 ≤ j ≤ i */

    /* -- compute the value (i·j) mod r -- */
    seq_t val = ((seq_t)i * (seq_t)j) % (seq_t)r;
    return val;
}
