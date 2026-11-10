/*
 * A287127  Positions of 1 in A287125; complement of A287126.
 * https://oeis.org/A287127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287125, the fixed point of
 *            00->001, 1->110 starting from 00 (computed by expanding the word
 *            until it holds n ones)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9872 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    unsigned char *curr = (unsigned char *)malloc(2);
    curr[0] = 0;
    curr[1] = 0;
    idx_t curr_len = 2;

    for (;;) {
        idx_t ones = 0;
        for (idx_t i = 0; i < curr_len; ++i) {
            if (curr[i]) {
                ++ones;
                if (ones == n) {
                    free(curr);
                    return (seq_t)(i + 1);
                }
            }
        }

        idx_t next_len = 0;
        unsigned char *next = (unsigned char *)malloc(3 * (size_t)curr_len);
        for (idx_t i = 0; i < curr_len; ++i) {
            unsigned char c = curr[i];
            if (c == 0 && i + 1 < curr_len && curr[i + 1] == 0) {
                next[next_len++] = 0;
                next[next_len++] = 0;
                next[next_len++] = 1;
                ++i;
            } else if (c == 1) {
                next[next_len++] = 1;
                next[next_len++] = 1;
                next[next_len++] = 0;
            } else {
                next[next_len++] = 0;
            }
        }
        free(curr);
        curr = next;
        curr_len = next_len;
    }
}
