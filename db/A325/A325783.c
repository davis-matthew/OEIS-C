/*
 * A325783  Reading the first row of this array, or the first column, or the
 *          successive antidiagonals is the same as reading this sequence.
 * https://oeis.org/A325783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Let  k = floor((sqrt(8*n + 1) – 1) / 2)  and  T = k*(k+1)/2.
 * If n = T          -> a(n) = a(k).
 * If n = T + 1      -> a(n) = a(k+1).
 * Otherwise (2 ≤ n – T ≤ k) -> a(n) = (k-2)*(k-1)/2 + (n-T) + 1.
 * Base: a(1) = 1, a(2) = 2.
 * This implementation uses an integer square‑root routine to avoid
 * floating‑point arithmetic.
 */

#include <stdbool.h>

/* Integer square root of a non‑negative 64‑bit integer.
 * Returns ⌊√x⌋.  Uses binary search. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    idx_t ans = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == x) {
            return mid;
        } else if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2) {
        return (seq_t)n;  /* a(1)=1, a(2)=2 */
    }

    while (1) {
        /* k = floor((sqrt(8*n + 1) – 1) / 2) */
        idx_t val = (idx_t)(((__uint128_t)n * 8) + 1);
        idx_t d   = isqrt_u64(val);
        idx_t k   = (d - 1) / 2;

        /* Triangular number T_k = k*(k+1)/2 */
        idx_t T = k * (k + 1) / 2;

        /* Position within the current antidiagonal */
        idx_t r = n - T;

        if (r == 0) {          /* n = T_k   →   a(n) = a(k) */
            n = k;
            if (n <= 2) return (seq_t)n;
            continue;
        }
        if (r == 1) {          /* n = T_k+1 →   a(n) = a(k+1) */
            n = k + 1;
            if (n <= 2) return (seq_t)n;
            continue;
        }
        /* 2 ≤ r ≤ k : direct formula */
        seq_t base = ((seq_t)(k - 2) * (k - 1)) / 2;
        return base + (seq_t)r + 1;
    }
}
