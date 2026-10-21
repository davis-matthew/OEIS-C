/*
 * A286764  Total number of nodes summed over all lattice paths from (0,0) to
 *          (n,n) that do not go below the x-axis or above the diagonal x=y
 *          and consist of steps D=(1,-1), H=(1,0) and S=(0,1).
 * https://oeis.org/A286764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total nodes summed over all lattice paths from (0,0) to
 *            (n,n) that stay in 0 ≤ y ≤ x using steps H=(1,0), S=(0,1) and
 *            D=(1,-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 0,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   We compute the number of such paths f(i,j) to each cell (i,j) and the total
   path lengths g(i,j).  Recurrence:
      f(i,j) = (i>0 && j≤i-1 ? f(i-1,j)   : 0)
            + (i>1 && j≤i-2 ? f(i-1,j+1) : 0)
            + (j>0             ? f(i,j-1) : 0);
      g(i,j) = (i>0 && j≤i-1 ? g(i-1,j)+f(i-1,j)   : 0)
            + (i>1 && j≤i-2 ? g(i-1,j+1)+f(i-1,j+1) : 0)
            + (j>0             ? g(i,j-1)+f(i,j-1) : 0);
   The answer a(n) is g(n,n) + f(n,n).
*/
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* a(0) = 1 */

    /* Allocate two rows for f and g. */
    seq_t *f_prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *f_cur  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *g_prev = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *g_cur  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!f_prev || !f_cur || !g_prev || !g_cur) {
        /* allocation failure – return 0 as safe fallback */
        return (seq_t)0;
    }

    f_prev[0] = (seq_t)1;   /* base: f(0,0)=1 */
    g_prev[0] = (seq_t)0;   /* g(0,0)=0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute row i into f_cur and g_cur */
        for (idx_t j = 0; j <= i; ++j) {
            seq_t fsum = (seq_t)0;
            seq_t gsum = (seq_t)0;

            /* from H: (i-1,j) */
            if (j <= i - 1) {
                fsum += f_prev[j];
                gsum += g_prev[j] + f_prev[j];
            }
            /* from D: (i-1,j+1) */
            if (j <= i - 2) {
                fsum += f_prev[j + 1];
                gsum += g_prev[j + 1] + f_prev[j + 1];
            }
            /* from S: (i,j-1) */
            if (j >= 1) {
                fsum += f_cur[j - 1];
                gsum += g_cur[j - 1] + f_cur[j - 1];
            }

            f_cur[j] = fsum;
            g_cur[j] = gsum;
        }

        /* swap current and previous rows for next iteration */
        seq_t *tmp = f_prev;
        f_prev = f_cur;
        f_cur  = tmp;

        tmp = g_prev;
        g_prev = g_cur;
        g_cur  = tmp;
    }

    seq_t result = g_prev[n] + f_prev[n];

    /* free allocated memory */
    free(f_prev);
    free(f_cur);
    free(g_prev);
    free(g_cur);

    return result;
}
