/*
 * A244933  Number of length n 0..1 arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming:
   a(n) = number of binary arrays of length n such that for all k,
   |S(k) - k/2| <= sqrt(3k)/2, equivalently (2S(k)-k)^2 <= 3k. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Integer square root for 64‑bit unsigned values */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t l = 0, h = (uint64_t)1 << 32;   /* sqrt(max uint64) < 2^32 */
    while (l < h) {
        uint64_t m = (l + h + 1) >> 1;
        if (m > 0 && m > x / m) h = m - 1;
        else l = m;
    }
    return l;
}

/* a(n) : OEIS A244933 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; n>=1 for the sequence */

    /* dp arrays for the previous prefix length */
    seq_t *dp_prev = (seq_t *)calloc(1, sizeof(seq_t));
    if (!dp_prev) return 0;
    dp_prev[0] = 1;                 /* one empty array with sum 0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* compute integer bound sqrt(3k) */
        uint64_t sqrt3k = isqrt_u64(3 * (uint64_t)k);

        /* lower bound */
        int64_t diff = (int64_t)k - (int64_t)sqrt3k;
        int64_t lowc = diff / 2;                 /* truncates toward zero */
        if (diff > 0 && (diff & 1)) lowc += 1;   /* ceil for positive odd */
        size_t low = (size_t)lowc;

        /* upper bound */
        size_t high = ((size_t)k + (size_t)sqrt3k) / 2;   /* floor */

        /* array for the new prefix length */
        size_t next_size = (size_t)k + 1;
        seq_t *dp_next = (seq_t *)calloc(next_size, sizeof(seq_t));
        if (!dp_next) { free(dp_prev); return 0; }

        for (size_t s = low; s <= high; ++s) {
            seq_t cnt0 = (s < k) ? dp_prev[s] : (seq_t)0;
            seq_t cnt1 = (s > 0) ? dp_prev[s - 1] : (seq_t)0;
            dp_next[s] = cnt0 + cnt1;
        }

        free(dp_prev);
        dp_prev = dp_next;
    }

    /* Sum over all admissible sums for length n */
    uint64_t sqrt3n = isqrt_u64(3 * (uint64_t)n);
    int64_t diffn = (int64_t)n - (int64_t)sqrt3n;
    int64_t lowcn = diffn / 2;
    if (diffn > 0 && (diffn & 1)) lowcn += 1;
    size_t low_n  = (size_t)lowcn;
    size_t high_n = ((size_t)n + (size_t)sqrt3n) / 2;

    seq_t result = 0;
    for (size_t s = low_n; s <= high_n; ++s)
        result += dp_prev[s];

    free(dp_prev);
    return result;
}
