/*
 * A316839  Indices of 2's in A254990.
 * https://oeis.org/A316839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0-based index of the n-th 2 in the 4-bonacci word A254990
 *            (fixed point of 0->01, 1->02, 2->03, 3->0);
 *            S(k)=S(k-1)S(k-2)S(k-3)S(k-4), found by descending through block
 *            lengths and counts of 2's
 * Range:     u8 n <= 36, u16 n <= 9151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9037 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t A316839_word[4][8] = {
    {0},
    {0, 1},
    {0, 1, 0, 2},
    {0, 1, 0, 2, 0, 1, 0, 3}
};
static const unsigned A316839_wlen[4] = {1, 2, 4, 8};

seq_t a(idx_t n)
{
    uint64_t len[100], cnt[100];
    int k;
    len[0] = 1; len[1] = 2; len[2] = 4; len[3] = 8;
    cnt[0] = 0; cnt[1] = 0; cnt[2] = 1; cnt[3] = 1;
    uint64_t r = (uint64_t)n;
    k = 3;
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
        if (j > 4) j = 4;
        k -= j;
    }
    uint64_t seen = 0;
    for (unsigned i = 0; i < A316839_wlen[k]; i++) {
        if (A316839_word[k][i] == 2) {
            seen++;
            if (seen == r) { pos += i; break; }
        }
    }
    return (seq_t)pos;
}
