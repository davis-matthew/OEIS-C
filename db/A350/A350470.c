/*
 * A350470  Array read by ascending antidiagonals. T(n, k) = J(k, n) where J
 *          are the Jacobsthal polynomials.
 * https://oeis.org/A350470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(s − pos, pos) where T(n,k)=Σ_{t=0..k} C(k−t,t)*(2n)^t
 *            and the index n is read from the matrix T by ascending
 *            antidiagonals
 * Range:     u8 n <= 50, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* integer ≥0 such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
static idx_t anti_diagonal(idx_t n)
{
    idx_t low = 0, high = n, s = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 total = (__int128)mid * (mid + 1) / 2;
        if (total <= n) {
            s  = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    /* locate the anti‑diagonal and the position inside it */
    idx_t s = anti_diagonal(n);
    idx_t prev = s * (s + 1) / 2;         /* terms before this diagonal   */
    idx_t pos = n - prev;                 /* 0‑based index in the diagonal */

    /* coordinates in the array T(i,j) = J(j,i) */
    idx_t i_idx = s - pos;                /* n in the Jacobsthal polynomial */
    idx_t j_idx = pos;                    /* k in the polynomial            */
    seq_t i = (seq_t)i_idx;
    seq_t j = (seq_t)j_idx;

    /* compute Σ_{t=0..j} C(j−t,t)·(2·i)^t */
    seq_t result = 0;
    seq_t power  = 1;                     /* (2·i)^t   */
    seq_t base   = ((seq_t)2) * i;        /* 2·i       */

    for (idx_t t = 0; t <= j_idx; ++t) {
        seq_t bin = binomial(j_idx - t, t);   /* C(j−t,t)   */
        result += bin * power;
        power *= base;
    }
    return result;
}
