/*
 * A309625  Number of even parts in the partitions of n into 7 parts.
 * https://oeis.org/A309625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of even parts in the partitions of n into exactly
 *            7 parts
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Trivial cases – fewer than 7 parts yields no partitions */
    if (n < 7)
        return (seq_t)0;

    const int K = 7;
    const size_t stride = (size_t)(n + 1);                 /* columns: sums 0..n   */
    const size_t total_cells = (size_t)(K + 1) * stride;   /* rows 0..K            */

#if defined(__cplusplus)
    /* C++: value‑initialise via new[] (calls constructors) */
    seq_t *count = new seq_t[total_cells]();
    seq_t *even  = new seq_t[total_cells]();
#else
    /* C: calloc gives zeroed memory (trivial types) */
    seq_t *count = (seq_t *)calloc(total_cells, sizeof(seq_t));
    seq_t *even  = (seq_t *)calloc(total_cells, sizeof(seq_t));
#endif
    if (!count || !even) {
        /* Allocation failure – return zero as safe default */
#if defined(__cplusplus)
        delete[] count;
        delete[] even;
#else
        free(count);
        free(even);
#endif
        return (seq_t)0;
    }

    /* Helper to compute linear index */
    #define IDX(k,s) ((size_t)(k) * stride + (s))

    /* Base case: zero parts sum to zero */
    count[IDX(0,0)] = (seq_t)1;
    even[IDX(0,0)]  = (seq_t)0;

    /* Dynamic programming over part values */
    for (idx_t val = 1; val <= n; ++val) {
        for (int k = 1; k <= K; ++k) {
            for (idx_t s = val; s <= n; ++s) {
                seq_t cntPrev = count[IDX(k-1, s-val)];
                if (cntPrev == 0)
                    continue;                     /* nothing to add */

                seq_t *cntCur  = &count[IDX(k, s)];
                seq_t *evenCur = &even[IDX(k, s)];

                *cntCur  += cntPrev;
                if (val & 1ULL) {                 /* part is odd */
                    *evenCur += even[IDX(k-1, s-val)];
                } else {                          /* part is even */
                    *evenCur += even[IDX(k-1, s-val)] + cntPrev;
                }
            }
        }
    }

    seq_t result = even[IDX(K, n)];

#if defined(__cplusplus)
    delete[] count;
    delete[] even;
#else
    free(count);
    free(even);
#endif
#undef IDX
    return result;
}
