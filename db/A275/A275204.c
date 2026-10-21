/*
 * A275204  Triangle read by rows: T(n,k) = number of domino stacks with n
 *          dominoes having a base of k dominoes.
 * https://oeis.org/A275204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 359, u32 n <= 2362, u64 n <= 9963,
 *            u128 n <= 9963, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the triangle:
 *    T(n,k) = Σ_{i=1..k} (2*(k-i)+1) * T(n-k,i)   with   T(n,n)=1
 *    T(n,k)=0 if n<k or k<=0 or n<=0.
 * Sequence a(n) enumerates the triangle row by row starting at n=1.
 * The triangle rows are bounded: row r contains r terms.
 * This implementation caches rows on demand using the recurrence above.
 * It works for any seq_t and idx_t provided by the build system.
 */
#include <stdlib.h>

/* cached triangle: triangle[t][k] (k=1..t). Index 0 unused. */
static seq_t **triangle = NULL;
static idx_t  computed_rows = 0;   /* highest row that has been computed */

/* Ensure that rows 1..max_row of the triangle are computed. */
static void ensure_rows(idx_t max_row)
{
    if (computed_rows >= max_row)
        return;

    /* extend the array of row pointers */
    if (triangle == NULL)
        triangle = (seq_t**)malloc((max_row + 1) * sizeof(seq_t*));
    else
        triangle = (seq_t**)realloc(triangle, (max_row + 1) * sizeof(seq_t*));
    if (triangle == NULL)
        return;                           // allocation failure – future calls will fail gracefully

    /* compute every missing row */
    for (idx_t t = computed_rows + 1; t <= max_row; ++t) {
        triangle[t] = (seq_t*)calloc(t + 1, sizeof(seq_t));
        if (triangle[t] == NULL)
            return;                               // allocation failure

        for (idx_t k = 1; k <= t; ++k) {
            if (k == t) {                       /* base case T(t,t)=1 */
                triangle[t][k] = (seq_t)1;
            } else {
                seq_t sum = 0;
                idx_t t_minus_k = t - k;
                for (idx_t i = 1; i <= k; ++i) {
                    if (t_minus_k >= i) {
                        seq_t term = triangle[t_minus_k][i];
                        seq_t mult = (seq_t)(2 * ((k - i)) + 1);
                        sum += mult * term;
                    }
                }
                triangle[t][k] = sum;
            }
        }
    }
    computed_rows = max_row;
}

/* Public function as required. Returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for index 0 */

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column index k within this row (1‑based). */
    idx_t prev_cumulative = cumulative - r;        /* terms up to row r-1 */
    idx_t k = n - prev_cumulative;                  /* k in [1..r] */

    ensure_rows(r);
    return triangle[r][k];
}
