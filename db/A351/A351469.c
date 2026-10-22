/*
 * A351469  Irregular triangle read by rows where row n is Adelman's sequence
 *          containing all permutations of 1..n.
 * https://oeis.org/A351469
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8643, u16 n <= 8643, u32 n <= 8643, u64 n <= 8643,
 *            u128 n <= 8643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A351469: Irregular triangle read by rows.
   The triangle has rows of length L(n)=n^2-2n+4 for n≥2 and is
   constructed by Adelman’s construction.  For a given global index
   n (offset 2), the element a(n) is the Kth entry of row r where
   r satisfies Σ_{i=2}^{r-1}L(i) < n-1 ≤ Σ_{i=2}^{r}L(i).  The entry
   is computed by the rules for T(r,K).  No hard‑coded table is used.  */
#include <stdint.h>
#include <stddef.h>

static inline __int128 cum_terms(idx_t r) {
    if (r < 2) return 0;
    idx_t m = r - 1;
    __int128 mm = m;
    /* Σ_{j=1}^{m} j^2  + 3m  */
    return mm * (mm + 1) * (2 * mm + 1) / 6 + 3 * mm;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* below offset, undefined */

    /* convert to zero‑based index into flattened triangle */
    idx_t idx = n - 2;

    /* find the row r such that cumulative(r-1) ≤ idx < cumulative(r) */
    idx_t lo = 2, hi = 2;
    while (cum_terms(hi) <= idx) hi <<= 1;              /* grow hi until big enough */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum_terms(mid) > idx) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prev = cum_terms(r - 1);
    idx_t k = idx - prev + 1;   /* 1‑based column within the row */

    /* compute T(r,k) according to Adelman construction */
    if (k == 1) return (seq_t)r;
    if (k == 2) return (seq_t)1;

    idx_t len = r * r - 2 * r + 4;   /* length of the row */

    if (k == len) return (seq_t)r;

    if (k == len - 1) {             /* special case at position w-1 */
        return (r == 2) ? (seq_t)1 : (seq_t)2;
    }

    /* general case for k between 3 and len-2 */
    idx_t div = (k - 2) / (r - 1);
    idx_t rem = (k - 2) % (r - 1);

    if (rem == 0) return (seq_t)r;

    /* (rem - div) mod (r-1), ensuring non‑negative */
    __int128 diff = (__int128)rem - (__int128)div;
    if (diff < 0) diff += (r - 1);

    return (seq_t)(1 + (seq_t)diff);
}
