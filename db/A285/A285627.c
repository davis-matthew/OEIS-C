/*
 * A285627  Positions of 1 in A285625; complement of A285626.
 * https://oeis.org/A285627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285625, the fixed point of the
 *            morphism 0->11, 1->1010 starting from "1" (computed by expanding
 *            the word and scanning)
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8927 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A285627_word = NULL;
static uint64_t A285627_len = 0;
static uint64_t A285627_ones = 0;

static void A285627_expand(void)
{
    uint64_t i, pos = 0, nl = 0;
    unsigned char *nw;
    uint64_t ones = 0;
    if (!A285627_word) {
        A285627_word = (unsigned char *)malloc(1);
        if (!A285627_word) abort();
        A285627_word[0] = 1;
        A285627_len = 1;
        A285627_ones = 1;
        return;
    }
    for (i = 0; i < A285627_len; i++) nl += A285627_word[i] ? 4 : 2;
    nw = (unsigned char *)malloc((size_t)nl);
    if (!nw) abort();
    for (i = 0; i < A285627_len; i++) {
        if (A285627_word[i]) {
            nw[pos++] = 1; nw[pos++] = 0; nw[pos++] = 1; nw[pos++] = 0;
            ones += 2;
        } else {
            nw[pos++] = 1; nw[pos++] = 1;
            ones += 2;
        }
    }
    free(A285627_word);
    A285627_word = nw;
    A285627_len = nl;
    A285627_ones = ones;
}

seq_t a(idx_t n)
{
    uint64_t i, cnt = 0;
    if (n == 0) return 0;
    while (!A285627_word || A285627_ones < (uint64_t)n) A285627_expand();
    for (i = 0; i < A285627_len; i++) {
        if (A285627_word[i]) {
            cnt++;
            if (cnt == (uint64_t)n) return (seq_t)(i + 1);
        }
    }
    return 0;
}
