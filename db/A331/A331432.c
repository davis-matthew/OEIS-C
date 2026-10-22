/*
 * A331432  Triangle T(n,k) (n >= k >= 0) read by rows: T(n,0) = (1+(-1)^n)/2;
 *          for k>=1, set T(0,k) = 0, S(n,k) =
 *          binomial(n,k)*binomial(n+k+1,k), and for n>=1, T(n,k) =
 *          S(n,k)-T(n-1,k).
 * https://oeis.org/A331432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 40, u32 n <= 128, u64 n <= 422,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A331432  Triangle T(n,k) read by rows.
    Definitions:
      T(n,0) = (1+(-1)^n)/2
      T(0,k) = 0  for k≥1
      S(n,k) = binomial(n,k) * binomial(n+k+1,k)
      T(n,k) = S(n,k) – T(n-1,k)   (n≥1, k≥1)
    The sequence a(n) is the row-major reading of T(n,k).
*/

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

static inline idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* grow high until it exceeds the row that contains n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n &&
           high < ((idx_t)-1) / 2)
        high <<= 1;

    /* binary search on [low, high)  for largest r with r(r+1)/2 <= n */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

static seq_t compute_T(idx_t r, idx_t k)
{
    if (k == 0)            /* T(r,0) = (1+(-1)^r)/2 */
        return (r & 1) ? 0 : 1;

    seq_t prev = 0;        /* T(0,k) = 0 for k>=1 */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t coeff = binomial(i, k);                       /* binom(i,k)   */
        seq_t t2    = binomial(i + k + 1, k);              /* binom(i+k+1,k) */
        seq_t s     = coeff * t2;                           /* S(i,k)       */
        seq_t cur   = s - prev;                            /* T(i,k)       */
        prev = cur;
    }
    return prev;   /* T(r,k) after loop */
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);                 /* row index */
    idx_t prev_sum = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev_sum;                /* column index within the row */

    return compute_T(r, k);
}
