/*
 * A300294  Irregular triangle giving the GCD characteristic: T(1, 1) = 1 and,
 *          for n >= 2 and 1 <= m <= n-1, T(n, m) = 1 if gcd(n, m) = 1 and 0
 *          otherwise.
 * https://oeis.org/A300294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 9611, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the irregular triangle T(n,m)
   defined in OEIS A300294.
   For a given index n (1‑based), it first determines the
   triangle row r and column m such that the n‑th term in
   row‑by‑row order corresponds to T(r,m), then returns
   1 if gcd(r,m)=1 (otherwise 0).  */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;      /* T(1,1) = 1 */

    /* Find the row r such that
       1 + (r-1)*r/2 >= n  and  1 + (r-2)*(r-1)/2 < n  */
    idx_t r = 1;
    for (;;) {
        idx_t cum = 1 + (r - 1) * r / 2;   /* total terms up to row r */
        if (cum >= n) break;
        ++r;
    }

    /* Compute the column m within this row */
    idx_t prev_cum = 1 + (r - 2) * (r - 1) / 2;   /* terms up to row r-1 */
    idx_t m = n - prev_cum;                      /* 1‑based column index */

    /* Return 1 if gcd(r,m) == 1 else 0 */
    seq_t g = gcd((seq_t)r, (seq_t)m);
    return (g == 1) ? 1 : 0;
}
