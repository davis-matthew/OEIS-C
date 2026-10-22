/*
 * A369580  a(n) := f(n, n), where f(0,0) = 1/3, f(0,k) = 0 and f(k,0) =
 *          3^(k-1) if k > 0, and f(n, m) = f(n, m-1) + f(n-1, m) + 3*f(n-1,
 *          m-1) otherwise.
 * https://oeis.org/A369580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,n) with f(0,0)=1/3, f(0,k)=0, f(k,0)=3^(k-1) (k>0),
 *            f(n,m)=f(n,m-1)+f(n-1,m)+3*f(n-1,m-1) for n,m>0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We compute g(n,m)=3*f(n,m) which is integer.
    Recurrence:  g(n,m)=g(n,m-1)+g(n-1,m)+3*g(n-1,m-1),
    initial rows:
          g(0,0)=1 ,  g(0,j>0)=0            (row 0)
          g(i>0,0)=3^i                         (column 0) */

/*  a(n) = g(n,n)/3.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset */

    /* convert to a type that safely indexes the arrays */
    size_t N = (size_t)n;

    /* allocate two rows of size N+1 each */
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failed – return 0 (sequence not defined) */
        free(prev); free(curr);
        return (seq_t)0;
    }

    /* base row i=0 */
    prev[0] = (seq_t)1;
    /* prev[j>0] are already zero by calloc */

    seq_t pow3 = (seq_t)1;        /* holds 3^i, starts with 3^0 */

    /* compute rows 1 .. N */
    for (size_t i = 1; i <= N; ++i) {
        /* update 3^i */
        pow3 *= (seq_t)3;
        curr[0] = pow3;                       /* g(i,0) = 3^i */

        /* compute rest of the row */
        for (size_t j = 1; j <= N; ++j) {
            /* g(i,j) = g(i,j-1) + g(i-1,j) + 3*g(i-1,j-1) */
            curr[j] = curr[j - 1] + prev[j] + ((seq_t)3 * prev[j - 1]);
        }

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* result: f(N,N) = g(N,N)/3 */
    seq_t result = prev[N] / (seq_t)3;

    /* free allocated memory */
    free(prev);
    free(curr);

    return result;
}
