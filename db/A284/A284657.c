/*
 * A284657  Positions of 0 in A284656; complement of A284658.
 * https://oeis.org/A284657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A284656, the limit word of
 *            Phi=theta^2 (Phi(0)=0110, Phi(1)=1011001101) started from 1;
 *            found by descending the expansion tree using zero counts
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8919 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int A284657_c0[4] = {0, 1, 1, 0};
static const int A284657_c1[10] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

seq_t a(idx_t n)
{
    uint64_t len0[64], len1[64], z0[64], z1[64];
    int depth = 0;
    len0[0] = len1[0] = 1;
    z0[0] = 1;
    z1[0] = 0;
    while (z1[depth] < (uint64_t)n) {
        int d = depth + 1;
        len0[d] = 2 * len0[depth] + 2 * len1[depth];
        len1[d] = 4 * len0[depth] + 6 * len1[depth];
        z0[d] = 2 * z0[depth] + 2 * z1[depth];
        z1[d] = 4 * z0[depth] + 6 * z1[depth];
        depth = d;
    }

    int symbol = 1;
    uint64_t k = (uint64_t)n;
    uint64_t offset = 0;

    while (depth > 0) {
        int cnt = (symbol == 0) ? 4 : 10;
        const int *ch = (symbol == 0) ? A284657_c0 : A284657_c1;
        for (int i = 0; i < cnt; ++i) {
            uint64_t zc = (ch[i] == 0) ? z0[depth - 1] : z1[depth - 1];
            if (k <= zc) {
                symbol = ch[i];
                depth--;
                break;
            }
            k -= zc;
            offset += (ch[i] == 0) ? len0[depth - 1] : len1[depth - 1];
        }
    }

    return (seq_t)(offset + 1);
}
