/*
 * A284658  Positions of 1 in A284656; complement of A284657.
 * https://oeis.org/A284658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284656 (found by scanning
 *            A284656 and counting its 1s)
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8594 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const int A284658_child0[4] = {0, 1, 1, 0};
static const int A284658_child1[10] = {1, 0, 1, 1, 0, 0, 1, 1, 0, 1};

static int A284658_word(idx_t n)
{
    idx_t len0[64];
    idx_t len1[64];
    int depth = 0;
    len0[0] = len1[0] = 1;
    while (len1[depth] < n) {
        int d = depth + 1;
        len0[d] = 2 * len0[depth] + 2 * len1[depth];
        len1[d] = 4 * len0[depth] + 6 * len1[depth];
        depth = d;
    }
    int symbol = 1;
    idx_t pos = n;
    while (depth > 0) {
        int cnt = (symbol == 0) ? 4 : 10;
        const int *ch = (symbol == 0) ? A284658_child0 : A284658_child1;
        for (int i = 0; i < cnt; ++i) {
            idx_t cl = (ch[i] == 0) ? len0[depth - 1] : len1[depth - 1];
            if (pos <= cl) {
                symbol = ch[i];
                depth--;
                break;
            }
            pos -= cl;
        }
    }
    return symbol;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (A284658_word(k) == 1) ++count;
    }
    return (seq_t)k;
}
