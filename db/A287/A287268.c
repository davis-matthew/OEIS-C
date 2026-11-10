/*
 * A287268  Positions of 0 in A287267.
 * https://oeis.org/A287268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A287267, the 1-limiting word of
 *            0->11, 1->02, 2->0; computed by iterating the 2-step morphism
 *            0->0202, 1->110, 2->11 from "11" and scanning
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9872 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *A287268_word = NULL;
static idx_t A287268_len = 0;
static idx_t A287268_zeros = 0;

static void A287268_grow(void)
{
    unsigned char *next = (unsigned char *)malloc((size_t)A287268_len * 4 + 4);
    idx_t out = 0, z = 0;
    for (idx_t i = 0; i < A287268_len; ++i) {
        unsigned char v = A287268_word[i];
        if (v == 0) {
            next[out++] = 0; next[out++] = 2; next[out++] = 0; next[out++] = 2;
            z += 2;
        } else if (v == 1) {
            next[out++] = 1; next[out++] = 1; next[out++] = 0;
            z += 1;
        } else {
            next[out++] = 1; next[out++] = 1;
        }
    }
    free(A287268_word);
    A287268_word = next;
    A287268_len = out;
    A287268_zeros = z;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    if (!A287268_word) {
        A287268_len = 2;
        A287268_word = (unsigned char *)malloc(2);
        A287268_word[0] = 1;
        A287268_word[1] = 1;
        A287268_zeros = 0;
    }
    while (A287268_zeros < n)
        A287268_grow();

    idx_t cnt = 0;
    for (idx_t i = 0; i < A287268_len; ++i) {
        if (A287268_word[i] == 0) {
            ++cnt;
            if (cnt == n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
