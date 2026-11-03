/*
 * A308674  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals, where
 *          A(n,k) is Sum_{d|n} d^(d^k).
 * https://oeis.org/A308674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 45 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A308674: a(n) = Sum_{d|i} d^(d^k), where (i,k) are the coordinates
   obtained by reading the infinite square array A(i,k) in antidiagonal order.
   The array is defined for i >= 1 (row) and k >= 0 (column). */

/* required utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

seq_t a(idx_t n)
{
    /* Convert the 1‑based index n to a 0‑based index for the utility. */
    idx_t n0 = n - 1;

    /* antidiagonal_coords returns a 0–based row index and k.
       Convert row to 1–based because A(i,k) definition uses i >= 1. */
    idx_t row0, k;
    antidiagonal_coords(n0, &row0, &k);
    idx_t i = row0 + 1;   /* actual row number */

    /* Sums over all divisors d of i :  d^(d^k) */
    seq_t sum = 0;

    for (uint64_t d = 1; d <= i / d; ++d) {
        if (i % d == 0) {
            /* exponent d^k */
            seq_t exp1 = ipow_i32((seq_t)d, (int)k);
            /* term d^(d^k) */
            seq_t term1 = ipow((seq_t)d, exp1);
            sum += term1;

            /* handle the paired divisor i/d if distinct */
            uint64_t d2 = i / d;
            if (d2 != d) {
                seq_t exp2 = ipow_i32((seq_t)d2, (int)k);
                seq_t term2 = ipow((seq_t)d2, exp2);
                sum += term2;
            }
        }
    }
    return sum;
}
