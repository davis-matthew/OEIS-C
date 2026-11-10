/*
 * A284956  Positions of 1 in A284954; complement of A284955.
 * https://oeis.org/A284956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284954, the 0-limiting word of
 *            the morphism 0->10, 1->000 (computed by iterating the morphism
 *            an even number of times)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9547 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t cur_len = 1;
    char *word = (char*)malloc(1);
    if (!word) return (seq_t)0;
    word[0] = 0;
    idx_t iter = 0;
    idx_t ones = 0;
    while ((iter & 1) || ones < n) {
        idx_t new_len = 2 * cur_len + ones;
        char *nw = (char*)malloc((size_t)new_len);
        if (!nw) { free(word); return (seq_t)0; }
        idx_t pos = 0;
        idx_t new_ones = 0;
        for (idx_t i = 0; i < cur_len; ++i) {
            if (word[i] == 0) {
                nw[pos++] = 1;
                nw[pos++] = 0;
                ++new_ones;
            } else {
                nw[pos++] = 0;
                nw[pos++] = 0;
                nw[pos++] = 0;
            }
        }
        free(word);
        word = nw;
        cur_len = new_len;
        ones = new_ones;
        ++iter;
    }
    idx_t cnt = 0;
    idx_t result = 0;
    for (idx_t i = 0; i < cur_len; ++i) {
        if (word[i] == 1) {
            ++cnt;
            if (cnt == n) { result = i + 1; break; }
        }
    }
    free(word);
    return (seq_t)result;
}
