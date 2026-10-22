/*
 * A348482  Triangle read by rows: T(n,k) = (Sum_{i=k..n} i!)/(k!) for 0 <= k
 *          <= n.
 * https://oeis.org/A348482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2,
 *            T(r,k) = (Σ_{i=k..r} i!) / k! (read the triangle by rows)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned integer.
   Returns ⌊√x⌋.  (binary method, O(log bits)) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;   /* the largest power of four ≤ 2^64 */
    while (bit > x) bit >>= 2;      /* find first bit ≤ x */

    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find row r and column k for linear index n. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) >> 1;          /* (√(8n+1)-1)/2 */
    idx_t before = r * (r + 1) / 2;                     /* elements before row */
    idx_t k = n - before;                               /* column in this row */

    /* Allocate factorial and prefix‑sum arrays up to r. */
    seq_t *fact = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!fact || !pref) {
        free(fact);
        free(pref);
        return 0;   /* allocation failure – caller must handle */
    }

    fact[0] = (seq_t)1;
    pref[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;   /* i! */
        pref[i] = pref[i - 1] + fact[i];    /* Σ_{j=0..i} j! */
    }

    /* Sum of factorials from k to r. */
    seq_t row_sum = pref[r];
    if (k > 0) row_sum -= pref[k - 1];

    seq_t result = row_sum / fact[k];   /* exact division by definition */

    free(fact);
    free(pref);
    return result;
}
