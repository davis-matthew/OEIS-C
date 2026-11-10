/*
 * A285531  Positions of 0 in A285530; complement of A285532.
 * https://oeis.org/A285531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A285530, the 0-limiting word of
 *            0->11, 1->0100 (computed by expanding the induced substitution
 *            0->01000100, 1->1101001111 and scanning)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 8893 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *A285531_w = NULL;
static size_t A285531_len = 0;

static void A285531_expand(void)
{
    static const unsigned char f0[8] = {0,1,0,0,0,1,0,0};
    static const unsigned char f1[10] = {1,1,0,1,0,0,1,1,1,1};
    if (A285531_len == 0) {
        A285531_w = (unsigned char *)malloc(1);
        if (!A285531_w) exit(1);
        A285531_w[0] = 0;
        A285531_len = 1;
        return;
    }
    size_t nl = 0;
    for (size_t i = 0; i < A285531_len; ++i)
        nl += A285531_w[i] == 0 ? 8 : 10;
    unsigned char *nw = (unsigned char *)malloc(nl);
    if (!nw) exit(1);
    size_t p = 0;
    for (size_t i = 0; i < A285531_len; ++i) {
        if (A285531_w[i] == 0) { memcpy(nw + p, f0, 8); p += 8; }
        else { memcpy(nw + p, f1, 10); p += 10; }
    }
    free(A285531_w);
    A285531_w = nw;
    A285531_len = nl;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    if (need == 0) return 0;
    for (;;) {
        size_t cnt = 0;
        for (size_t i = 0; i < A285531_len; ++i) {
            if (A285531_w[i] == 0) {
                if (++cnt == need) return (seq_t)(i + 1);
            }
        }
        A285531_expand();
    }
}
