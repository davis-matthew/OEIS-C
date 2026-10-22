/*
 * A330827  a(n) is the numbers of ways to write 2*n = u + v where the ternary
 *          representations of u and of v have the same number of digits d for
 *          d = 0..2.
 * https://oeis.org/A330827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{(u,v) | u+v = 2*n, ternary(u) and ternary(v) are
 *            anagrams}|
 * Range:     u8 n <= 475, u16 n <= 9272, u32 n <= 9272, u64 n <= 9272,
 *            u128 n <= 9272, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts *ordered* pairs, so each (u,v) is counted separately.
   This matches the OEIS A330827 sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct { uint8_t c0, c1, c2; } DigCounts;

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n * 2;           /* 2*n as a seq_t */
    if (s == 0) return (seq_t)1;      /* only 0+0 */

    /* Compute digit counts for all numbers 0 .. s */
    size_t max_k = (size_t)s;         /* s fits into size_t for the intended range */
    DigCounts *cnt = (DigCounts *)malloc((max_k + 1) * sizeof(DigCounts));
    if (!cnt) return (seq_t)0;        /* insufficient memory */

    /* 0 has representation "0": one zero digit */
    cnt[0].c0 = 1; cnt[0].c1 = 0; cnt[0].c2 = 0;

    for (size_t k = 1; k <= max_k; ++k) {
        uint8_t c0 = 0, c1 = 0, c2 = 0;
        uint64_t tmp = k;                 /* k fits in 64‑bit for the tests */
        while (tmp) {
            switch (tmp % 3) {
                case 0: ++c0; break;
                case 1: ++c1; break;
                default: ++c2; break;   /* case 2 */
            }
            tmp /= 3;
        }
        cnt[k].c0 = c0; cnt[k].c1 = c1; cnt[k].c2 = c2;
    }

    seq_t ans = 0;
    for (size_t u = 0; u <= max_k; ++u) {
        size_t v = max_k - u;
        DigCounts *au = &cnt[u];
        DigCounts *av = &cnt[v];
        if (au->c0 == av->c0 && au->c1 == av->c1 && au->c2 == av->c2)
            ++ans;
    }

    free(cnt);
    return ans;
}
