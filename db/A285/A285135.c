/*
 * A285135  Positions of 1 in A285133; complement of A285134.
 * https://oeis.org/A285135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A285133, the fixed point of the
 *            morphism s^2 with s: 0->10, 1->0001, starting from 0 (computed
 *            by generating the word and scanning)
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9899 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned char *A285135_w = NULL;
static size_t A285135_len = 0;

static void A285135_grow(void)
{
    size_t i, p = 0, nl = 0;
    unsigned char *nw;
    if (A285135_w == NULL) {
        A285135_w = (unsigned char *)malloc(1);
        A285135_w[0] = 0;
        A285135_len = 1;
    }
    /* length of s^2(w): 0 -> 6 letters, 1 -> 10 letters */
    for (i = 0; i < A285135_len; i++) nl += A285135_w[i] ? 10 : 6;
    nw = (unsigned char *)malloc(nl);
    for (i = 0; i < A285135_len; i++) {
        if (A285135_w[i] == 0) {
            /* 0 -> 10 -> 0001 10 = 000110 */
            static const unsigned char b0[6] = {0,0,0,1,1,0};
            memcpy(nw + p, b0, 6); p += 6;
        } else {
            /* 1 -> 0001 -> 10 10 10 0001 */
            static const unsigned char b1[10] = {1,0,1,0,1,0,0,0,0,1};
            memcpy(nw + p, b1, 10); p += 10;
        }
    }
    free(A285135_w);
    A285135_w = nw;
    A285135_len = nl;
}

seq_t a(idx_t n)
{
    static size_t ones = 0;      /* number of ones in current word */
    size_t target = (size_t)n;
    size_t i;
    for (;;) {
        if (A285135_w != NULL) {
            ones = 0;
            for (i = 0; i < A285135_len; i++) {
                if (A285135_w[i]) {
                    ones++;
                    if (ones == target) return (seq_t)(i + 1);
                }
            }
        }
        A285135_grow();
    }
}
