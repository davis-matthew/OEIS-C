/*
 * A287270  Positions of 2 in A287267.
 * https://oeis.org/A287270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 2 in A287267, the 1-limiting word of
 *            0->11, 1->02, 2->0 (found by generating the word and scanning)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9533 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287270_word = NULL;
static idx_t A287270_len = 0;
static idx_t A287270_twos = 0;

static void A287270_count(void)
{
    idx_t c = 0;
    for (idx_t i = 0; i < A287270_len; ++i)
        if (A287270_word[i] == 2) c++;
    A287270_twos = c;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    if (!A287270_word) {
        A287270_len = 2;
        A287270_word = (unsigned char *)malloc(2);
        A287270_word[0] = 1;
        A287270_word[1] = 1;
        A287270_count();
    }

    while (A287270_twos < n) {
        unsigned char *next = (unsigned char *)malloc((size_t)A287270_len * 4 + 4);
        idx_t out = 0;
        for (idx_t i = 0; i < A287270_len; ++i) {
            unsigned char v = A287270_word[i];
            if (v == 0) {
                next[out++] = 0; next[out++] = 2;
                next[out++] = 0; next[out++] = 2;
            } else if (v == 1) {
                next[out++] = 1; next[out++] = 1; next[out++] = 0;
            } else {
                next[out++] = 1; next[out++] = 1;
            }
        }
        free(A287270_word);
        A287270_word = next;
        A287270_len = out;
        A287270_count();
    }

    idx_t c = 0;
    for (idx_t i = 0; i < A287270_len; ++i) {
        if (A287270_word[i] == 2) {
            c++;
            if (c == n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
