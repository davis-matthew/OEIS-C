/*
 * A244899  Number of length n 0..4 arrays with each partial sum starting from
 *          the beginning no more than sqrt(2) standard deviations from its
 *          mean.
 * https://oeis.org/A244899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays of integers 0..4 such that for
 *            each prefix of length k, the partial sum S_k satisfies |S_k -
 *            2k| <= 2*sqrt(k). The computation is performed by dynamic
 *            programming over the possible partial sums
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64. */
static inline idx_t int_sqrt_u64(idx_t n)
{
    idx_t x = 0;                 /* running result */
    idx_t bit = (idx_t)1 << 62;  /* the second highest bit of 64‑bit numbers */

    /* Find the largest power of 4 <= n */
    while (bit > n) bit >>= 2;

    for (; bit != 0; bit >>= 2)
    {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
    }
    return x;
}

/* Count of admissible arrays of length n */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset starts at 1; guard for completeness */
        return 0;

    const idx_t maxsum = 4 * n;          /* maximum possible partial sum */
    const size_t  arrlen = (size_t)maxsum + 1;

    seq_t *prev = (seq_t *)malloc(arrlen * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(arrlen * sizeof(seq_t));
    if (!prev || !curr) {
        /* In a contest environment we should not exit, but
           returning 0 is a safe fallback when allocation fails. */
        if (prev) free(prev);
        if (curr) free(curr);
        return 0;
    }
    memset(prev, 0, arrlen * sizeof(seq_t));
    memset(curr, 0, arrlen * sizeof(seq_t));

    prev[0] = 1;                     /* empty prefix has sum 0 */

    idx_t low_prev  = 0;
    idx_t high_prev = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Allowed deviation: floor(2*sqrt(k)) = floor(sqrt(4k)) */
        idx_t dmax = int_sqrt_u64(4 * k);

        idx_t low_k = (2 * k > dmax) ? (2 * k - dmax) : 0;
        idx_t high_k = 2 * k + dmax;           /* always <= 4*k */

        /* clear current array */
        memset(curr, 0, arrlen * sizeof(seq_t));

        /* Transition from prev sums to curr sums */
        for (idx_t s = low_prev; s <= high_prev; ++s) {
            seq_t cnt = prev[s];
            if (cnt == 0) continue;
            for (int v = 0; v <= 4; ++v) {
                idx_t snew = s + v;
                if (snew >= low_k && snew <= high_k)
                    curr[snew] += cnt;
            }
        }

        /* swap prev and curr */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        low_prev  = low_k;
        high_prev = high_k;
    }

    /* Sum all counts in the final allowed range */
    seq_t result = 0;
    for (idx_t s = low_prev; s <= high_prev; ++s)
        result += prev[s];

    free(prev);
    free(curr);
    return result;
}
