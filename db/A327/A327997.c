/*
 * A327997  Triangle read by rows: coefficients of the polynomials given by
 *          KummerU(-n, 1 - n - x, 3).
 * https://oeis.org/A327997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{i=0}^{r} C(r,i) * 3^i * S1(r-i, k) where r =
 *            ⌊(√(8n+1)-1)/2⌋ and k = n - r(r+1)/2. S1(m,k) – unsigned
 *            Stirling numbers of the first kind
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Compute row r and column k that locate the flat index n in the
       triangle.  r satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       ------------------------------------------------------------------ */
    long double t = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t     r = ((idx_t)t - 1) / 2;                     /* initial guess */

    /* Adjust r in case of rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;

    idx_t k = n - r * (r + 1) / 2;   /* column index (0‑based) */

    /* ------------------------------------------------------------------
       Allocate temporary arrays for Stirling numbers and for binomial
       coefficients / powers of 3.  Size is r+1 (maximum index needed).
       ------------------------------------------------------------------ */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *pow3 = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *bin =  (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !curr || !pow3 || !bin) return 0; /* should never happen */

    /* ------------------------------------------------------------------
       Initialise column 0 of Stirling numbers – S1(m,0) = 1 if m==0 else 0.
       ------------------------------------------------------------------ */
    prev[0] = 1;
    for (idx_t i = 1; i <= r; ++i) prev[i] = 0;

    /* ------------------------------------------------------------------
       Compute column k (S1(m,k) for m = 0..r) while only keeping two
       columns at a time (prev and curr).  Use recurrence:
           S1(u,k) = S1(u-1,k-1) + (u-1)*S1(u-1,k)
       ------------------------------------------------------------------ */
    for (idx_t step = 1; step <= k; ++step) {
        curr[0] = 0;                     /* S1(0,step) = 0 for step>0 */
        for (idx_t u = 1; u <= r; ++u) {
            seq_t term = prev[u - 1];          /* S1(u-1,step-1) */
            seq_t mult = curr[u - 1];          /* S1(u-1,step)  */
            mult = mult * (seq_t)(u - 1);      /* (u-1)*S1(u-1,step) */
            curr[u] = term + mult;             /* S1(u,step)   */
        }
        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    /* After loop, 'prev' holds S1(u,k) for all u=0..r. */

    /* ------------------------------------------------------------------
       Pre‑compute powers of 3 and binomial coefficients C(r,i).
       ------------------------------------------------------------------ */
    pow3[0] = 1;
    for (idx_t i = 1; i <= r; ++i) pow3[i] = pow3[i - 1] * 3;

    bin[0] = 1;
    for (idx_t i = 1; i <= r; ++i) {
        bin[i] = bin[i - 1] * (seq_t)(r - i + 1) / (seq_t)i;
    }

    /* ------------------------------------------------------------------
       Sum over u = 0..r : i = r - u.  Each term is
           C(r,i) * 3^i * S1(u,k)
       ------------------------------------------------------------------ */
    seq_t res = 0;
    for (idx_t u = 0; u <= r; ++u) {
        idx_t i = r - u;
        seq_t term = bin[i] * pow3[i] * prev[u];
        res += term;
    }

    /* ------------------------------------------------------------------
       Clean up and return the result.
       ------------------------------------------------------------------ */
    free(prev);
    free(curr);
    free(pow3);
    free(bin);

    return res;
}
