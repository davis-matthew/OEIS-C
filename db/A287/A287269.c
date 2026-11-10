/*
 * A287269  Positions of 1 in A287267.
 * https://oeis.org/A287269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A287267 (1-limiting word of
 *            0->11, 1->02, 2->0); computed by generating the word and
 *            scanning
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *A287269_word = NULL;
static idx_t A287269_len = 0;
static idx_t *A287269_pos = NULL;
static idx_t A287269_cnt = 0;

static void A287269_scan(void)
{
    free(A287269_pos);
    A287269_pos = (idx_t *)malloc((size_t)(A287269_len + 1) * sizeof(idx_t));
    A287269_cnt = 0;
    for (idx_t i = 0; i < A287269_len; ++i)
        if (A287269_word[i] == 1)
            A287269_pos[A287269_cnt++] = i + 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    if (!A287269_word) {
        A287269_len = 2;
        A287269_word = (unsigned char *)malloc(2);
        A287269_word[0] = 1;
        A287269_word[1] = 1;
        A287269_scan();
    }

    while (A287269_cnt < n) {
        unsigned char *next = (unsigned char *)malloc((size_t)A287269_len * 4);
        idx_t out = 0;
        for (idx_t i = 0; i < A287269_len; ++i) {
            unsigned char v = A287269_word[i];
            if (v == 0) {
                next[out++] = 0; next[out++] = 2;
                next[out++] = 0; next[out++] = 2;
            } else if (v == 1) {
                next[out++] = 1; next[out++] = 1; next[out++] = 0;
            } else {
                next[out++] = 1; next[out++] = 1;
            }
        }
        free(A287269_word);
        A287269_word = next;
        A287269_len = out;
        A287269_scan();
    }

    return (seq_t)A287269_pos[n - 1];
}
