/*
 * A317501  Triangle read by rows: T(0,0) = 1; T(n,k) = 2*T(n-1,k) +
 *          T(n-4,k-1) for k = 0..floor(n/4); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) is the n-th entry of the triangle defined by T(n,k) =
 *            2^(n-4k) * C(n-3k, k) (for 0 <= k <= floor(n/4)), read
 *            row‑by‑row in the order T(0,0), T(1,0), T(2,0), T(3,0), T(4,0),
 *            T(4,1), T(5,0), T(5,1), T(6,0), T(6,1), …
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is not defined by a simple recurrence for a(n); it is
   derived from the triangle reading order. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* cumulative number of terms in the triangle up to and including row r */
static inline idx_t cumulative_entries(idx_t r)
{
    idx_t q   = r / 4;          /* floor(r/4)  */
    idx_t rem = r % 4;          /* r mod 4     */
    idx_t sum_floor;
    if (q == 0)
        sum_floor = 0;
    else
        sum_floor = q * ((idx_t)2 * q + rem - 1);   /* 2*q*(q-1) + (rem+1)*q */
    return r + 1 + sum_floor;
}

/* return the n-th term of A317501 */
extern seq_t a(idx_t n)
{
    /* find the row r that contains the n-th entry (0‑based) */
    idx_t low = 0, high = 1;
    while (cumulative_entries(high) <= n)
        high <<= 1;                     /* double until high is large enough */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (cumulative_entries(mid) > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* terms before this row */
    idx_t before = (r == 0) ? 0 : cumulative_entries(r - 1);

    /* column index within the row */
    idx_t k = n - before;

    /* compute T(r,k) = 2^(r-4k) * C(r-3k, k) */
    idx_t exp = r - 4 * k;                /* r >= 4k always holds */
    idx_t a_val = r - 3 * k;              /* a = r-3k for the binomial */
    seq_t term = pow2(exp) * binomial(a_val, k);
    return term;
}
