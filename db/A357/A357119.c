/*
 * A357119  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} |Stirling1(n,k*j)|.
 * https://oeis.org/A357119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i, k) where i = n - i*(i+1)/2 and k = i - (n -
 *            i*(i+1)/2); * T(i, k) = Sum_{j=0..⌊i/k⌋} |Stirling1(i, k*j)|,
 *            Stirling1 = unsigned Stirling numbers of the first kind
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"

static idx_t max_stirling_n = 0;      /* largest row index already computed   */
static seq_t **stirling = NULL;       /* tabulated unsigned Stirling numbers */

/*-------------------------------------------------------------------
 * Ensure Stirling numbers s1(n,k) are computed for all n <= nmax.
 * Row n stores values for k = 0 .. n; k > n is 0 by definition.
 *-------------------------------------------------------------------*/
static void compute_stirling_up_to(idx_t nmax)
{
    if (stirling == NULL) {                     /* allocate row 0 */
        stirling = (seq_t **)malloc(sizeof(seq_t *));
        if (!stirling) return;                  /* unlikely but safe */
        stirling[0] = alloc_seq_array(1);        /* row 0 */
        stirling[0][0] = 1;                     /* |s(0,0)| = 1 */
        max_stirling_n = 0;
    }
    if (nmax <= max_stirling_n) return;

    stirling = (seq_t **)realloc(stirling, (nmax + 1) * sizeof(seq_t *));
    if (!stirling) return;                      /* unlikely but safe */

    for (idx_t n = max_stirling_n + 1; n <= nmax; ++n) {
        seq_t *row = alloc_seq_array((size_t)n + 1);
        row[0] = 0;                              /* |s(n,0)| = 0 for n > 0 */
        row[n] = 1;                              /* |s(n,n)| = 1 */
        if (n > 1) {
            seq_t *prev = stirling[n - 1];
            for (idx_t k = 1; k < n; ++k) {
                /* |s(n,k)| = |s(n-1,k-1)| + (n-1)*|s(n-1,k)|   */
                seq_t term1 = prev[k - 1];
                seq_t term2 = prev[k] * (seq_t)(n - 1);
                row[k] = term1 + term2;
            }
        }
        stirling[n] = row;
    }
    max_stirling_n = nmax;
}

/*-------------------------------------------------------------------
 * T(n,k) = Sum_{j=0..⌊n/k⌋} |s(n, k*j)|.
 *---------------------------------------------------------------*/
static seq_t T_value(idx_t n, idx_t k)
{
    if (k == 0) return (n == 0) ? (seq_t)1 : (seq_t)0;
    seq_t sum = 0;
    for (idx_t j = 0; k * j <= n; ++j) {
        sum += stirling[n][k * j];
    }
    return sum;
}

/*-------------------------------------------------------------------
 * Find the diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2.
 *---------------------------------------------------------------*/
static idx_t diagonal_of(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid;
        else          high = mid - 1;
    }
    return low;
}

/*-------------------------------------------------------------------
 * Public API: compute a(n).
 *---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t s = diagonal_of(n);                    /* diagonal index */
    idx_t prev = (s * (s + 1)) >> 1;           /* elements before this diagonal */
    idx_t i = n - prev;                       /* row index within diagonal (0‑based) */
    idx_t k = s - i;                           /* column index within diagonal */

    compute_stirling_up_to(i);                 /* need Stirling numbers up to row i */
    return T_value(i, k);
}
