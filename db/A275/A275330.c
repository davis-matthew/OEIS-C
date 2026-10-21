/*
 * A275330  Triangle read by rows, T(n,k) = t(n-k+1)*Sum_{d|k} d*t(d) with t =
 *          A000081, for n>=1 and 1<=k<=n.
 * https://oeis.org/A275330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = t[n-k+1] * Sum_{d|k} d*t[d] where t = A000081. a(n) is
 *            the triangle read by rows, i.e. the nth entry in row-major order
 *            of T
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for realloc */

static seq_t *t = NULL;       /* t[1..max_n]  */
static seq_t *S = NULL;       /* S[1..max_n]  */
static idx_t max_n = 0;

/* Compute a(n).  Calls may come in any order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; values below are irrelevant */

    /* Expand the tables if necessary */
    if (max_n < n) {
        idx_t old = max_n;
        idx_t newmax = n;

        /* Reallocate memory */
        t = (seq_t *)realloc(t, (newmax + 1) * sizeof(seq_t));
        S = (seq_t *)realloc(S, (newmax + 1) * sizeof(seq_t));

        /* Initialise new entries of S to 0 (t is not needed yet) */
        for (idx_t i = old + 1; i <= newmax; ++i)
            S[i] = 0;

        /* Add contributions from old divisors to the new indices */
        for (idx_t d = 1; d <= old; ++d) {
            idx_t j = (old / d + 1) * d;          /* first multiple of d > old */
            for (; j <= newmax; j += d)
                S[j] += (seq_t)d * t[d];
        }

        /* Compute t for the new range [old+1 .. newmax] */
        for (idx_t m = old + 1; m <= newmax; ++m) {
            if (m == 1) {
                t[1] = 1;
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k < m; ++k)
                    sum += S[k] * t[m - k];
                t[m] = sum / (seq_t)(m - 1);   /* exact division */
            }
            /* Update S for multiples of m */
            for (idx_t j = m; j <= newmax; j += m)
                S[j] += (seq_t)m * t[m];
        }

        max_n = newmax;
    }

    /* Convert the flat index n into (row, column) */
    idx_t low  = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* mid*(mid+1)/2 may overflow idx_t for very large n,
           but idx_t is 64‑bit so it is safe for n up to 10^10 */
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;
    idx_t prev_rows = (row - 1) * row / 2;
    idx_t k = n - prev_rows;               /* column in the row   */

    /* Return T(row,k) */
    return t[row - k + 1] * S[k];
}
