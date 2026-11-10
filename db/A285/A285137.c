/*
 * A285137  Positions of 0 in A285136; complement of A285138.
 * https://oeis.org/A285137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285136, the limiting word of
 *            0->10, 1->0001 (built by iterating f(0)=000110, f(1)=1010100001
 *            from 1, then scanning)
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9540 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t N = (size_t)n;
    size_t L = 2 * N + 16;

    uint8_t *cur = (uint8_t *)malloc(L);
    if (!cur)
        return (seq_t)0;
    cur[0] = 1;
    size_t cur_len = 1;

    static const uint8_t f0[6] = {0, 0, 0, 1, 1, 0};
    static const uint8_t f1[10] = {1, 0, 1, 0, 1, 0, 0, 0, 0, 1};

    while (cur_len < L) {
        uint8_t *next = (uint8_t *)malloc(L);
        if (!next) {
            free(cur);
            return (seq_t)0;
        }
        size_t nl = 0;
        for (size_t i = 0; i < cur_len && nl < L; ++i) {
            if (cur[i] == 0) {
                for (size_t k = 0; k < 6 && nl < L; ++k)
                    next[nl++] = f0[k];
            } else {
                for (size_t k = 0; k < 10 && nl < L; ++k)
                    next[nl++] = f1[k];
            }
        }
        free(cur);
        cur = next;
        cur_len = nl;
    }

    size_t zeros = 0;
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cur_len; ++i) {
        if (cur[i] == 0) {
            ++zeros;
            if (zeros == N) {
                result = (seq_t)(i + 1);
                break;
            }
        }
    }
    free(cur);
    return result;
}
