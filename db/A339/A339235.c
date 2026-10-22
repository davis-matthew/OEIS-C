/*
 * A339235  G.f.: Sum_{k>=0} x^(k^4) / Product_{j=1..k^4} (1 - x^j).
 * https://oeis.org/A339235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n such that the number of parts
 *            is a fourth power (k^4) (OEIS A339235). Computed by dynamic
 *            programming for the partition function p_r(n) = number of
 *            partitions of n into exactly r parts using the recurrence
 *            p(0,0)=1 , p(n,0)=0 (n>0) p(n,r)=0 (n<r) p(n,r)=p(n-1,r-1)...
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate arrays large enough for the requested n */
    idx_t N = n;
    seq_t *prev = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *ans  = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    char   *fourth = (char *)calloc((N + 1), sizeof(char));
    if (!prev || !curr || !ans || !fourth) return 0; /* allocation failure */

    /* mark all fourth powers up to N (including 0) */
    for (idx_t i = 0; ; ++i) {
        unsigned long long p = (unsigned long long)i * i;
        p *= p;  /* p = i^4 */
        if (p > (unsigned long long)N) break;
        fourth[(idx_t)p] = 1;
    }

    /* initial conditions */
    prev[0] = 1;          /* p(0,0)=1 */
    for (idx_t i = 1; i <= N; ++i) prev[i] = 0;
    ans[0] = 1;           /* only the 0-part partition contributes for n=0 */
    for (idx_t i = 1; i <= N; ++i) ans[i] = 0;

    /* dynamic programming over part count k */
    for (idx_t k = 1; k <= N; ++k) {
        curr[0] = 0;
        for (idx_t i = 1; i <= N; ++i) {
            if (i < k) {
                curr[i] = 0;
            } else {
                /* curr[i] = curr[i - k] + prev[i - 1] */
                curr[i] = curr[i - k] + prev[i - 1];
            }
        }

        /* if k is a fourth power, add its contribution to a(n) */
        if (fourth[k]) {
            for (idx_t i = 0; i <= N; ++i) {
                ans[i] = ans[i] + curr[i];
            }
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = ans[N];

    /* free temporary memory */
    free(prev);
    free(curr);
    free(ans);
    free(fourth);

    return result;
}
