/*
 * A308704  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(k*d+1).
 * https://oeis.org/A308704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 31, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308704: Square array A(n,k) read by antidiagonals,
   where A(n,k) = Sum_{d|n} d^(k*d+1).  This function computes
   the sequence a(n) in that order. */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe fallback */

    /* 1. Find diagonal S such that S*(S+1)/2 >= n.  Use binary search
       with 128‑bit intermediate to avoid overflow. */
    idx_t low = 1, high = 1;
    while (((__int128)high * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t S = low;

    /* 2. Position within diagonal (0‑based).  Cumulative before S is
       (S-1)*S/2. */
    idx_t cum_before = (idx_t)(((__int128)(S - 1) * S) / 2);
    idx_t pos = n - cum_before - 1;           /* 0‑based position */
    idx_t row = pos + 1;                      /* row index n >= 1 */
    idx_t k   = S - row;                      /* column index k >= 0 */

    /* 3. Compute sum of d^(k*d+1) over all divisors d of row. */
    seq_t sum = 0;

    for (idx_t d = 1; d <= row / d; ++d) {
        if (row % d == 0) {
            /* divisor d */
            seq_t exp_d  = (seq_t)k * (seq_t)d + 1;
            sum += big_pow((seq_t)d, exp_d);

            /* paired divisor row/d if distinct */
            if (d != row / d) {
                idx_t d2 = row / d;
                seq_t exp_d2 = (seq_t)k * (seq_t)d2 + 1;
                sum += big_pow((seq_t)d2, exp_d2);
            }
        }
    }
    return sum;
}
