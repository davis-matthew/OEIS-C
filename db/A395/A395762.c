/*
 * A395762  Array read by ascending antidiagonals: A(n, k) = Sum_{j=0..n}
 *          Stirling2(n, j)*5^j*(k)_j.
 * https://oeis.org/A395762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the array A(n,k) with * A(n,k)=∑_{j=0..n}
 *            Stirling2(n,j)*5^j*(k)_j. * Recurrence used: * A(0,k)=1 for all
 *            k≥0 * A(n,0)=0 for n>0 * A(n+1,k)=k*(A(n,k)+4*A(n,k-1)) for k≥1
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find sum index s such that s(s+1)/2 ≤ n < (s+1)(s+2)/2 */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    idx_t total_before = s * (s + 1) / 2;    /* terms before diagonal s */
    idx_t d = n - total_before;             /* offset inside diagonal */
    idx_t n_pair = s - d;                   /* n coordinate of term */
    idx_t k_pair = d;                       /* k coordinate of term */

    /* Allocate rows for DP.  k_pair+1 columns are needed. */
    if (k_pair == (idx_t)-1)  /* sanity check, should not happen */
        return 0;

    seq_t *prev = (seq_t *)malloc((k_pair + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((k_pair + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failed – return 0 as a fallback */
        free(prev);
        free(curr);
        return 0;
    }

    /* Base row: n = 0 */
    for (idx_t k = 0; k <= k_pair; ++k)
        prev[k] = 1;   /* A(0,k)=1 */

    /* Build rows up to n_pair */
    for (idx_t n_cur = 1; n_cur <= n_pair; ++n_cur) {
        curr[0] = 0;  /* A(n,0)=0 for n>0 */
        for (idx_t k = 1; k <= k_pair; ++k) {
            seq_t term = prev[k] + 4 * prev[k - 1];
            curr[k] = ((seq_t)k) * term;
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[k_pair];

    free(prev);
    free(curr);
    return result;
}
