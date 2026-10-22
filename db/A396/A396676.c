/*
 * A396676  Square array A(n,k), n>=1, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = n! * [x^n] -log(1 + W_k(-x)) and W_k(x) is the k-th
 *          iterate of LambertW(x).
 * https://oeis.org/A396676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A396676:  square array A(n,k) with
   A(n,0) = (n-1)!                 (n>=1)
   A(n,k) = Σ_{j=1..n} n^(n-j) * C(n-1,j-1) * A(j,k-1)   (k>0)
   a(n) is obtained by reading the array along the antidiagonals
   downward:  a(1)=A(1,0), a(2)=A(1,1), a(3)=A(2,0), a(4)=A(1,2), ...
   Initial values: 1, 1, 1, 1, 3, 2, 1, 5, 17, 6, 1, 7, 44, 142, ...   */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

#ifndef MAX_N
#  define MAX_N 250         /* sufficient for n up to ≈10 000 */
#endif

static seq_t A[MAX_N + 1][MAX_N + 1];   /* pre‑allocated table */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* guard against zero, though offset is 1 */

    /* determine the antidiagonal d that contains the nth term */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n) ++d;

    idx_t n_max = d + 1;                /* maximum n needed for this d */
    if (n_max > MAX_N) return 0;       /* safety check, shouldn’t happen */

    /* pre‑compute A(n,k) for 1 <= n <= n_max, 0 <= k <= d-(n-1) */
    static seq_t powv[MAX_N + 1];       /* reusable power array */
    for (idx_t i = 1; i <= n_max; ++i)
    {
        idx_t kmax_i = d - (i - 1);
        if (kmax_i > d) kmax_i = d;

        /* pre‑compute i^e for e = 0 .. i-1 */
        powv[0] = 1;
        for (idx_t e = 1; e < i; ++e)
            powv[e] = powv[e - 1] * (seq_t)i;

        /* base case k = 0 */
        if (i == 1) A[i][0] = 1;
        else        A[i][0] = factorial(i - 1);

        /* recurrence for k > 0 */
        for (idx_t k = 1; k <= kmax_i; ++k)
        {
            seq_t sum = 0;
            for (idx_t j = 1; j <= i; ++j)
            {
                seq_t term = powv[i - j];
                term *= binomial(i - 1, j - 1);
                term *= A[j][k - 1];
                sum += term;
            }
            A[i][k] = sum;
        }
    }

    /* traverse the antidiagonals in the correct order (n increasing) */
    idx_t pos = 0;
    for (idx_t diag = 0; diag <= d; ++diag)
    {
        for (idx_t i = 0; i <= diag; ++i)
        {
            idx_t n_cur = i + 1;          /* n = i+1 */
            idx_t k_cur = diag - i;       /* k = diag - i */
            ++pos;
            if (pos == n) return A[n_cur][k_cur];
        }
    }

    return 0;   /* should never reach here */
}
