/*
 * A394783  Triangle read by rows: T(n,k) = n^(k-1)*(k - 1)! with 1 <= k <= n.
 * https://oeis.org/A394783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m+1, k) where * m = floor((sqrt(8*(n-1)+1)-1)/2) * k =
 *            n - m*(m+1)/2 * T(p, q) = p^(q-1)*(q-1)! * * This gives OEIS
 *            A394783
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 35, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Return the largest m such that m*(m+1)/2 <= x */
static idx_t _floor_m(idx_t x)
{
    idx_t low = 0, high = 1;
    /* Grow high until it exceeds the bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= x) {
        high <<= 1;          /* high *= 2 */
    }
    /* Binary search */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n < 1, return 0 */
    idx_t n_minus1 = n - 1;
    idx_t m = _floor_m(n_minus1);                 /* m such that T_m <= n-1 < T_{m+1} */
    idx_t t = (unsigned __int128)m * (m + 1) / 2;   /* T_m */
    idx_t k = n - t;                           /* 1 <= k <= m+1 */
    seq_t p = (seq_t)(m + 1);                  /* base */
    seq_t power = ipow_idx(p, k - 1);           /* (m+1)^(k-1) */
    seq_t factorial = fact_upto(k - 1);        /* (k-1)! */
    return seq_mul(power, factorial);          /* product */
}
