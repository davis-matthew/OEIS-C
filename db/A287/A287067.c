/*
 * A287067  Positions of 0 in A287066.
 * https://oeis.org/A287067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the fixed word of 0->01, 1->12,
 *            2->0 started from 1 (A287066); computed by expanding the word
 *            until it has n zeros
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8547 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A287067_w = NULL;
static idx_t A287067_len = 0;
static idx_t *A287067_pos = NULL;
static idx_t A287067_cnt = 0;

static void A287067_grow(idx_t n)
{
    if (A287067_len == 0) {
        A287067_w = (unsigned char *)malloc(1);
        A287067_w[0] = 1;
        A287067_len = 1;
        A287067_cnt = 0;
    }
    while (A287067_cnt < n) {
        idx_t old = A287067_len;
        unsigned char *nb = (unsigned char *)malloc((size_t)(old * 2 + 2));
        idx_t k = 0;
        for (idx_t i = 0; i < old; i++) {
            unsigned char v = A287067_w[i];
            if (v == 0) { nb[k++] = 0; nb[k++] = 1; }
            else if (v == 1) { nb[k++] = 1; nb[k++] = 2; }
            else { nb[k++] = 0; }
        }
        free(A287067_w);
        A287067_w = nb;
        A287067_len = k;
        idx_t c = 0;
        for (idx_t i = 0; i < k; i++) if (nb[i] == 0) c++;
        free(A287067_pos);
        A287067_pos = (idx_t *)malloc((size_t)(c + 1) * sizeof(idx_t));
        c = 0;
        for (idx_t i = 0; i < k; i++) if (nb[i] == 0) A287067_pos[c++] = i + 1;
        A287067_cnt = c;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    A287067_grow(n);
    return (seq_t)A287067_pos[n - 1];
}
