/*
 * A373324  E.g.f. satisfies A(x) = exp(x) + x*A(x)^4.
 * https://oeis.org/A373324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A373324: E.g.f. satisfies A(x) = exp(x) + x*A(x)^4
   Recurrence:
       a(0) = 1
       a(n) = 1 + n * coeff_{x^(n-1)} A(x)^4   (n ≥ 1)
   The coefficient of x^(m)/m! in A(x)^p is
       c_m = Σ_{i1+…+ip=m} C(m;i1,…,ip) * a_{i1}…a_{ip},
   which for p=2 reduces to an ordinary convolution with binomial
   coefficients.  This implementation builds the values successively
   using two cumulated convolutions (A^2 and A^4).  It runs in
   O(n^2) time and uses O(n) memory.  All arithmetic is performed on
   seq_t as required by the build system.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- allocate workspace --- */
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)1;  /* a(0) = 1 */

    seq_t *a_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *h_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *g_arr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *binom = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    /* --- initialise base case --- */
    a_arr[0] = 1;
    h_arr[0] = 1;   /* coefficient of x^0 in A^2 is 1 */
    g_arr[0] = 1;   /* coefficient of x^0 in A^4 is 1 */

    /* ----- binom[0] = 1 for row 0 ----- */
    binom[0] = 1;

    /* --- compute a(1)…a(N) progressively --- */
    for (size_t cur = 1; cur <= N; ++cur) {
        /* build binomial row C(cur,i), i=0..cur */
        binom[cur] = 1;
        for (size_t i = cur; i > 0; --i) {
            if (i < cur) /* update inner terms only */
                binom[i] = binom[i] + binom[i - 1];
        }
        /* now binom[0..cur] contains the correct row */

        /* a[cur] = 1 + cur * g[cur-1] */
        seq_t temp = g_arr[cur - 1] * (seq_t)cur;
        a_arr[cur] = 1 + temp;

        /* h[cur] = coefficient of x^cur in A^2:
           h[cur] = Σ_{i=0}^{cur} C(cur,i) * a[i] * a[cur-i] */
        h_arr[cur] = 0;
        for (size_t i = 0; i <= cur; ++i) {
            seq_t prod = a_arr[i] * a_arr[cur - i];
            prod = prod * binom[i];
            h_arr[cur] += prod;
        }

        /* g[cur] = coefficient of x^cur in A^4:
           g[cur] = Σ_{i=0}^{cur} C(cur,i) * h[i] * h[cur-i] */
        g_arr[cur] = 0;
        for (size_t i = 0; i <= cur; ++i) {
            seq_t prod = h_arr[i] * h_arr[cur - i];
            prod = prod * binom[i];
            g_arr[cur] += prod;
        }
    }

    /* result */
    seq_t result = a_arr[N];

    /* free memory */
    free(a_arr);
    free(h_arr);
    free(g_arr);
    free(binom);

    return result;
}
