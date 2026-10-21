/*
 * A241334  Number of partitions p of n including floor(mean(p)) as a part.
 * https://oeis.org/A241334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} P(n - ⌊n/k⌋, k-1) where P(m, r) is the number
 *            of partitions of m into exactly r parts
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                     /* a(0) = 0 by definition */
    }

    /* convert index to size_t for array indexing */
    const size_t N = (size_t)n;

    /* allocate two working arrays for dynamic programming */
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!prev || !curr) {
        /* allocation failed – return 0 as safe fall back */
        return 0;
    }

    /* base case: partitions of 0 into 0 parts = 1 */
    prev[0] = 1;

    seq_t ans = 0;

    /* contribution from k = 1 (s = 0) */
    ans += prev[N - N / 1];           /* N - N = 0, so prev[0] */

    /* iterate over the number of parts s = k-1, from 1 to N-1 */
    for (size_t s = 1; s <= N - 1; ++s) {
        /* compute P(i, s) for all i = 0..N */
        for (size_t i = 0; i <= N; ++i) {
            seq_t val = 0;
            if (i >= 1) {
                val += prev[i - 1];          /* P(i-1, s-1) */
            }
            if (i >= s) {
                val += curr[i - s];          /* P(i-s, s) */
            }
            curr[i] = val;
        }

        /* current k = s + 1 */
        size_t k = s + 1;
        size_t need = N - N / k;             /* m = floor(N/k) → need = N - m */

        ans += curr[need];

        /* prepare for next iteration: swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* free temporary arrays */
    free(prev);
    free(curr);

    return ans;
}
