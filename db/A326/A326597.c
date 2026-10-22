/*
 * A326597  Sum of the second largest parts of the partitions of n into 10
 *          parts.
 * https://oeis.org/A326597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{r=1..⌊n/10⌋} Sum_{q=r..⌊(n-r)/9⌋} ...
 *            Sum_{i=j..⌊(n-j‑…)/2⌋} i. This implementation computes the sum
 *            of the second largest parts of partitions of n into 10 positive
 *            parts by counting non‑decreasing 10‑tuples. The algorithm runs
 *            in O(n) time and O(n) memory
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* partitions into 10 positive parts only exist for n >= 10 */
    if (n < 10) return 0;
    idx_t N = n - 10;           /* remaining weight after the mandatory +10 */
    size_t size = (size_t)N + 1;/* number of dp states (0 … N)   */

    /* Allocate DP arrays for counts and sums of processed variables.  */
    seq_t *count = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *sum   = (seq_t *)malloc(size * sizeof(seq_t));
    if (!count || !sum) {
        /* In the unlikely event of allocation failure, return 0. */
        if (count) free(count);
        if (sum)   free(sum);
        return 0;
    }

    /* Initialise: only the zero‑weight solution exists. */
    for (idx_t i = 0; i <= N; ++i) {
        count[i] = 0;
        sum[i]   = 0;
    }
    count[0] = 1;   /* one empty tuple */

    /* Weights of the first nine variables (b1', d1', …, d8'): */
    const idx_t weights[9] = {10, 9, 8, 7, 6, 5, 4, 3, 2};

    for (int wi = 0; wi < 9; ++wi) {
        idx_t w = weights[wi];
        for (idx_t s = w; s <= N; ++s) {
            seq_t cPrev = count[s - w];
            if (cPrev == 0) continue;
            count[s] += cPrev;
            /* For each old solution, adding one copy of this variable
               increases its count of processed components by 1. */
            sum[s] += sum[s - w] + cPrev;
        }
    }

    /* The weight‑1 variable (x9) takes whatever remainder is left;
       thus a partition exists for every weighted sum <= N. */
    seq_t totalCount = 0;
    seq_t totalSum   = 0;
    for (idx_t s = 0; s <= N; ++s) {
        totalCount += count[s];
        totalSum   += sum[s];
    }

    free(count);
    free(sum);

    /* Each partition contributes its counted sum plus 1 (for the +1
       in the expression a9 = b1' + … + d8' + 1). */
    return totalSum + totalCount;
}
