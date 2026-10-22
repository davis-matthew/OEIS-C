/*
 * A308930  Sum of the fourth largest parts in the partitions of n into 7
 *          parts.
 * https://oeis.org/A308930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{o=1..floor(n/7)} Sum_{m=o..floor((n-o)/6)}
 *            Sum_{l=m..floor((n-m-o)/5)} Sum_{k=l..floor((n-l-m-o)/4)}
 *            Sum_{j=k..floor((n-k-l-m-o)/3)}
 *            Sum_{i=j..floor((n-j-k-l-m-o)/2)} k
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const idx_t weights[7] = { 7, 6, 5, 4, 3, 2, 1 };

seq_t a(idx_t n) {
    if (n < 7)
        return (seq_t)0;

    idx_t N = n - 7;
    size_t size = (size_t)(N + 1);

    /* Allocate arrays for counts and sum of the first four d's. */
    seq_t *cnt = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *sum4 = (seq_t *)calloc(size, sizeof(seq_t));
    if (!cnt || !sum4) {
        /* Memory allocation failed – return 0; caller should not rely on this case. */
        if (cnt) free(cnt);
        if (sum4) free(sum4);
        return (seq_t)0;
    }

    cnt[0] = (seq_t)1;  /* Empty solution corresponds to the partition with all parts 1. */

    for (int j = 0; j < 7; ++j) {
        idx_t w = weights[j];
        int delta = (j < 4) ? 1 : 0;   /* d1..d4 contribute to the sum. */

        for (idx_t s = w; s <= N; ++s) {
            size_t idx = (size_t)(s - w);
            seq_t addCnt = cnt[idx];
            cnt[(size_t)s] += addCnt;
            if (delta) {
                /* Every new item of this weight increases d_j by 1, so add its contribution. */
                sum4[(size_t)s] += sum4[idx] + addCnt;
            } else {
                sum4[(size_t)s] += sum4[idx];
            }
        }
    }

    seq_t result = cnt[(size_t)N] + sum4[(size_t)N];

    free(cnt);
    free(sum4);
    return result;
}
