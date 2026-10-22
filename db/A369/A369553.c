/*
 * A369553  Expansion of g.f. A(x) satisfying A(x) = A( x^2*(1+x)^3 ) / x.
 * https://oeis.org/A369553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A369553
    The sequence satisfies    A(x) = A(x^2*(1+x)^3) / x
    a(1) = 1
    a(n) is computed for n≥1 from the triangular
    recurrence derived from the functional equation.
*/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /*  For n = 0 we don't have a term; return 0. */
    if (n == 0) return (seq_t)0;

    /*  Allocate storage for terms up to n.  */
    idx_t N = n;
    idx_t size = N + 2;          /*  we need indexes up to N+1 for x^{n+1}  */

    /*  Working arrays  */
    seq_t *a_vals   = (seq_t*)calloc(size, sizeof(seq_t));   /*  a[1..N]  */
    seq_t *ans      = (seq_t*)calloc(size, sizeof(seq_t));   /*  accumulator  */
    seq_t *prev_row = (seq_t*)calloc(size, sizeof(seq_t));   /*  coefficients for y^{k-1}  */
    seq_t *curr_row = (seq_t*)calloc(size, sizeof(seq_t));   /*  coefficients for y^k    */

    if (!a_vals || !ans || !prev_row || !curr_row)
        return 0;    /*  out of memory – just return 0, the build will never hit this  */

    /*  Base: a(1) = 1  */
    a_vals[1] = (seq_t)1;

    /*  Initially prev_row corresponds to y^0 = 1  */
    prev_row[0] = (seq_t)1;   /*  other entries are already 0  */

    /*  The outer loop iterates over k = 1 .. N-1  */
    for (idx_t k = 1; k <= N - 1; ++k) {

        /*  compute curr_row = y^k  */
        memset(curr_row, 0, size * sizeof(seq_t));
        for (idx_t m = 2 * k; m <= N + 1; ++m) {
            seq_t val = 0;
            val += prev_row[m - 2];
            val += 3 * ((m >= 3) ? prev_row[m - 3] : (seq_t)0);
            val += 3 * ((m >= 4) ? prev_row[m - 4] : (seq_t)0);
            val += ((m >= 5) ? prev_row[m - 5] : (seq_t)0);
            curr_row[m] = val;
        }

        /*  update the accumulator for all n that receive a contribution
            from the current k   */
        idx_t start = (2 * k > k) ? (2 * k - 1) : k;   /*  max(k, 2k-1)  */
        for (idx_t idx = start; idx <= N; ++idx) {
            ans[idx] += a_vals[k] * curr_row[idx + 1];
        }

        /*  Now we have all contributions for a_{k+1}  */
        a_vals[k + 1] = ans[k + 1];

        /*  move to the next power of y  */
        seq_t *tmp = prev_row;
        prev_row = curr_row;
        curr_row = tmp;
    }

    /*  free temporary arrays  */
    free(prev_row);
    free(curr_row);
    free(ans);

    /*  result */
    return a_vals[n];
}
