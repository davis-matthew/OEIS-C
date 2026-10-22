/*
 * A367680  Number of integer compositions x1+x2+...+xk of n such that each xj
 *          has exactly j bits set.
 * https://oeis.org/A367680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the number of compositions x1+...+xk = n
   where the j‑th part xj has exactly j 1‑bits in its binary
   representation.  The values are computed by dynamic programming
   over the admissible parts. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n = 0 → empty composition only */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Largest j such that 2^j – 1 <= n.  For larger j no part can
       appear because the smallest j‑bit number already exceeds n. */
    idx_t jmax = 0;
    idx_t pow  = 1;
    while (pow <= n)
    {
        if (pow > (UINT64_MAX >> 1))
            break;                 /* avoid wrap‑around, safe for given limits */
        pow <<= 1;
        ++jmax;
    }

    /* ---- 1. bits‑count of all numbers up to n ---- */
    unsigned char *bitcnt = (unsigned char *)malloc((size_t)(n + 1) *
                                                      sizeof(unsigned char));
    if (!bitcnt)
        return (seq_t)0;
    bitcnt[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        bitcnt[i] = bitcnt[i >> 1] + (unsigned char)(i & 1);

    /* ---- 2. number of values having j bits for j <= jmax ---- */
    size_t *cnt = (size_t *)calloc((size_t)(jmax + 2), sizeof(size_t));
    if (!cnt) { free(bitcnt); return (seq_t)0; }

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned char bc = bitcnt[i];
        if (bc <= jmax)
            ++cnt[bc];
    }

    /* ---- 3. allocate arrays to hold numbers with each bit count ---- */
    idx_t **vals = (idx_t **)malloc((size_t)(jmax + 2) *
                                      sizeof(idx_t *));
    size_t *len  = (size_t *)calloc((size_t)(jmax + 2), sizeof(size_t));
    if (!vals || !len) { free(bitcnt); free(cnt); return (seq_t)0; }

    for (idx_t j = 1; j <= jmax; ++j)
    {
        if (cnt[j])
        {
            vals[j] = (idx_t *)malloc(cnt[j] * sizeof(idx_t));
            if (!vals[j]) { free(bitcnt); free(cnt); free(vals); free(len);
                return (seq_t)0; }
            len[j] = cnt[j];
        }
        else
        {
            vals[j] = NULL;
            len[j]  = 0;
        }
    }

    /* ---- 4. fill value lists ---- */
    size_t *pos = (size_t *)calloc((size_t)(jmax + 2), sizeof(size_t));
    if (!pos) { free(bitcnt); free(cnt); free(vals); free(len);
        return (seq_t)0; }

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned char bc = bitcnt[i];
        if (bc <= jmax)
            vals[bc][pos[bc]++] = i;
    }
    free(pos);                    /* positions no longer needed */

    /* ---- 5. DP: two rows ---- */
    size_t N = (size_t)n + 1;
    seq_t *dp_next = (seq_t *)calloc(N, sizeof(seq_t));
    seq_t *dp_cur   = (seq_t *)malloc(N * sizeof(seq_t));
    if (!dp_next || !dp_cur)
    {
        free(bitcnt); free(cnt); free(vals); free(len);
        free(dp_next); free(dp_cur);
        return (seq_t)0;
    }

    dp_next[0] = (seq_t)1;        /* empty sum for parts beyond jmax */

    for (idx_t j = jmax; j >= 1; --j)
    {
        dp_cur[0] = (seq_t)1;    /* composition may terminate with remainder 0 */

        idx_t *vptr = vals[j];
        size_t   l   = len[j];

        for (idx_t rem = 1; rem <= n; ++rem)
        {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < l; ++k)
            {
                idx_t v = vptr[k];
                if (v > rem)
                    break;
                sum += dp_next[rem - v];
            }
            dp_cur[rem] = sum;
        }

        /* swap dp_cur and dp_next for the next iteration */
        seq_t *tmp = dp_next;
        dp_next = dp_cur;
        dp_cur = tmp;
    }

    seq_t result = dp_next[n];   /* final row holds the answer */

    /* ---- cleanup ---- */
    free(dp_cur);
    free(dp_next);
    for (idx_t j = 1; j <= jmax; ++j)
        if (vals[j]) free(vals[j]);
    free(vals);
    free(len);
    free(cnt);
    free(bitcnt);

    return result;
}
