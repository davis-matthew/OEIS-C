/*
 * A379342  a(n) = A378684(A378684(n)).
 * https://oeis.org/A379342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = A378684(A378684(n))
   Equivalent direct formula via the triangular array T(n,k):

   For a given index n (1‑based):
   1. Find the row r such that
        C(r-1) < n ≤ C(r) ,
      where C(r) = Σ_{i=1}^{r} (4i-3) = 2r^2 - r.
   2. Let k = n - C(r-1)  (1 ≤ k ≤ 4r-3).
   3. Compute the offset A000384(r-1) = (r-1)*(2(r-1)-1).
   4. Compute
        P = { max(k, 4r-3-k)   if k is odd
              min(k-1, 4r-2-k) if k is even } .
   5. Result = offset + P.

   All arithmetic is done in `seq_t` or `idx_t` as appropriate; no
   floating‑point or external helper functions are required. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* find row r  (1‑based) */
    idx_t r = 1;
    while (( (seq_t)2 * r * r - (seq_t)r ) < n)
        ++r;

    /* previous total up to row r-1 */
    idx_t m = r - 1;
    idx_t prev = 2 * m * m - m;      /* C(r-1) */

    /* position within the row */
    idx_t k = n - prev;               /* 1 ≤ k ≤ 4r-3 */

    /* base offset A000384(r-1) = (r-1)*(2(r-1)-1) */
    seq_t base = (seq_t)m * ( (seq_t)2 * m - 1 );

    /* compute P */
    idx_t P;
    if (k & 1) {        /* odd k */
        idx_t a = k;
        idx_t b = 4 * r - 3 - k;
        P = (a > b) ? a : b;
    } else {            /* even k */
        idx_t a = k - 1;
        idx_t b = 4 * r - 2 - k;
        P = (a < b) ? a : b;
    }

    /* result */
    return base + (seq_t)P;
}
