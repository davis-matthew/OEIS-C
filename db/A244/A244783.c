/*
 * A244783  Number of length n 0..3 arrays with each partial sum starting from
 *          the beginning no more than one standard deviation from its mean.
 * https://oeis.org/A244783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A244783
 * Number of length‑n 0..3 arrays with each partial sum s_k satisfying
 * |s_k – 3k/2| ≤ sqrt(5k)/2.  The DP works on the transformed
 * variable t_k = 2s_k – 3k, which satisfies |t_k| ≤ floor(sqrt(5k))
 * and increments by one of {‑3, ‑1, 1, 3} at each step.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        idx_t nxt = r | bit;
        if (nxt <= n / nxt)        /* avoid overflow */
            r = nxt;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset is 1 – a(0) is undefined */

    /* bound for the maximum prefix length n */
    idx_t maxBound = isqrt_u64(5 * n);
    size_t sz = (size_t)(2 * maxBound + 1);
    int off = (int)maxBound;          /* offset to map t to array index */

    /* dynamic allocation of DP rows */
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(sz, sizeof(seq_t));
    if (cur == NULL || nxt == NULL) {
        free(cur);
        free(nxt);
        return (seq_t)0;
    }

    /* initial state: t_0 = 0 */
    cur[off] = (seq_t)1;

    /* possible changes of t per array element */
    const int deltas[4] = { -3, -1, 1, 3 };

    for (idx_t k = 1; k <= n; ++k) {
        idx_t bound = isqrt_u64(5 * k);
        int bound_i = (int)bound;

        /* reset next row */
        for (size_t i = 0; i < sz; ++i)
            nxt[i] = (seq_t)0;

        for (size_t idx = 0; idx < sz; ++idx) {
            seq_t cnt = cur[idx];
            if (cnt == (seq_t)0)
                continue;

            int t = (int)idx - off;
            for (int d = 0; d < 4; ++d) {
                int r = t + deltas[d];
                if (r < -bound_i || r > bound_i)
                    continue;
                size_t pos = (size_t)(r + off);
                nxt[pos] += cnt;
            }
        }

        /* swap current and next rows */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all counts for the final length n */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < sz; ++i)
        result += cur[i];

    free(cur);
    free(nxt);
    return result;
}
