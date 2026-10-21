/*
 * A279004  Irregular triangle read by rows: generalized Catalan triangle
 *          C_3(n,k).
 * https://oeis.org/A279004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the irregular triangle read by rows.
 *            The triangle is the generalized Catalan triangle C_3(r,k), whose
 *            entries for 0 ≤ k are given by C_3(r,k) = binom(r + k, k) –
 *            binom(r + k, k – 3) (with binom(a,b)=0 when b<0 or b>a). The
 *            triangle rows have lengths r+3; row indices r...
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the provided binomial helper.
   It returns a seq_t value for given idx_t arguments. */
#include "../utils/binom.h"

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative count of elements up to
       that row exceeds n.  The cumulative count for row r is
           S(r) = (r+1)*(r+6)/2
       which is the total number of entries in rows 0..r.
       We find the smallest r with S(r) > n.  */
    idx_t low  = 0;
    idx_t high = n;           /* r can never exceed n (each row has ≥3 entries) */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Compute S(mid) using __uint128_t to avoid overflow */
        __uint128_t mid_u = (__uint128_t)mid;
        __uint128_t s_mid = (mid_u + 1) * (mid_u + 6) / 2;
        if (s_mid > n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;   /* row index */

    /* First index of row r is S(r-1) = r*(r+5)/2 */
    __uint128_t r_u   = (__uint128_t)r;
    __uint128_t first = r_u * (r_u + 5) / 2;
    idx_t first_idx  = (idx_t)first;  /* conversion safe because first ≤ n */

    /* Column within the row */
    idx_t k = n - first_idx;

    /* Compute the entry using the binomial formula */
    seq_t term1 = binom(r + k, k);          /* binom(n+k, k) */
    seq_t term2 = 0;
    if (k >= 3) {
        term2 = binom(r + k, k - 3);        /* binom(n+k, k-3) */
    }

    return term1 - term2;
}
