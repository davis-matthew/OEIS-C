/*
 * A316840  Indices of 3's in A254990.
 * https://oeis.org/A316840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0-based position of the n-th 3 in the 4-bonacci word
 *            A254990 (fixed point of 0->01, 1->02, 2->03, 3->0); found by
 *            descending S(k) = S(k-1)S(k-2)S(k-3)S(k-4) using lengths and
 *            counts of 3's
 * Range:     u8 n <= 18, u16 n <= 4747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8264 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t len[100], cnt[100];
    int k;
    len[0] = 1; len[1] = 2; len[2] = 4; len[3] = 8;
    cnt[0] = 0; cnt[1] = 0; cnt[2] = 0; cnt[3] = 1;
    k = 3;
    uint64_t r = (uint64_t)n;
    while (cnt[k] < r && k < 99) {
        k++;
        len[k] = len[k-1] + len[k-2] + len[k-3] + len[k-4];
        cnt[k] = cnt[k-1] + cnt[k-2] + cnt[k-3] + cnt[k-4];
    }
    uint64_t pos = 0;
    while (k > 3) {
        int j;
        for (j = 1; j <= 4; j++) {
            if (r <= cnt[k - j]) break;
            r -= cnt[k - j];
            pos += len[k - j];
        }
        if (j > 4) break; /* should not happen */
        k -= j;
    }
    pos += 7;
    return (seq_t)pos;
}
