/*
 * A380886  Triangle T(n,k), 1<=k<=n: column k are the coefficients of the
 *          INVERT transform of Sum_{i=1..k} i*x^i.
 * https://oeis.org/A380886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r is the row such that r*(r-1)/2 < n ≤
 *            r*(r+1)/2 and c = n - r*(r-1)/2. T(n,k) is the coefficient of
 *            x^n in 1/(1 - Σ_{i=1}^k i x^i)
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for all computed triangle values.  Tcache[r][k] holds T(r,k). */
static seq_t **Tcache   = NULL;   /* array of row pointers            */
static idx_t  Tmax     = 0;       /* highest row that has been computed */
static idx_t  Twidth   = 0;       /* column width of each stored row   */

/* Rebuild the entire triangle up to row 'maxR' (inclusive). */
static void rebuild_triangle(idx_t maxR)
{
    idx_t newWidth = maxR + 1;                 /* columns 0..maxR          */

    /* Allocate row pointers. */
    seq_t **newCache = (seq_t **)malloc((maxR + 1) * sizeof(seq_t *));
    if (!newCache) return;                    /* allocation failure        */

    /* Allocate each row and zero it. */
    for (idx_t r = 0; r <= maxR; ++r) {
        newCache[r] = (seq_t *)calloc(newWidth, sizeof(seq_t));
        if (!newCache[r]) {                   /* allocation failure        */
            for (idx_t j = 0; j < r; ++j) free(newCache[j]);
            free(newCache);
            return;
        }
    }

    /* Set the base row T(0,k) = 1 for all k. */
    for (idx_t k = 0; k < newWidth; ++k)
        newCache[0][k] = (seq_t)1;

    /* Compute all other entries.  For each n,k we use the recurrence:
       T(n,k) = Σ_{i=1}^{min(k,n)} i * T(n-i,k)   */
    for (idx_t n = 1; n <= maxR; ++n) {
        for (idx_t k = 1; k <= maxR; ++k) {
            seq_t sum = (seq_t)0;
            idx_t imax = (k < n) ? k : n;
            for (idx_t i = 1; i <= imax; ++i) {
                sum += (seq_t)i * newCache[n - i][k];
            }
            newCache[n][k] = sum;
        }
    }

    /* Free old cache if it existed. */
    if (Tcache) {
        for (idx_t r = 0; r <= Tmax; ++r) free(Tcache[r]);
        free(Tcache);
    }

    /* Replace old cache with the new one. */
    Tcache = newCache;
    Tmax   = maxR;
    Twidth = newWidth;
}

/* Return the nth term of OEIS sequence A380886. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, n=0 is unused */

    /* Determine the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column within the row (1‑based). */
    idx_t c = n - r * (r - 1) / 2;

    /* Ensure the triangle has been computed up to row r. */
    if (r > Tmax) rebuild_triangle(r);

    return Tcache[r][c];
}
