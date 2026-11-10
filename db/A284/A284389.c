/*
 * A284389  Positions of 0 in A284388; complement of A284390.
 * https://oeis.org/A284389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in the fixed point of psi: 0->001,
 *            1->11001 (A284388), found by iterating the morphism and scanning
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8409 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned char *A284389_w = NULL;
static size_t A284389_len = 0;
static size_t A284389_zeros = 0;

static void A284389_grow(void)
{
    size_t nl = 0, i, pos = 0, z = 0;
    unsigned char *nw;
    for (i = 0; i < A284389_len; ++i)
        nl += (A284389_w[i] == 0) ? 3 : 5;
    nw = (unsigned char *)malloc(nl);
    for (i = 0; i < A284389_len; ++i) {
        if (A284389_w[i] == 0) {
            nw[pos++] = 0; nw[pos++] = 0; nw[pos++] = 1;
        } else {
            nw[pos++] = 1; nw[pos++] = 1; nw[pos++] = 0; nw[pos++] = 0; nw[pos++] = 1;
        }
    }
    for (i = 0; i < nl; ++i)
        if (nw[i] == 0) ++z;
    free(A284389_w);
    A284389_w = nw;
    A284389_len = nl;
    A284389_zeros = z;
}

seq_t a(idx_t n)
{
    size_t i, cnt = 0;
    if (n == 0) return (seq_t)0;
    if (!A284389_w) {
        A284389_w = (unsigned char *)malloc(1);
        A284389_w[0] = 0;
        A284389_len = 1;
        A284389_zeros = 1;
    }
    while ((idx_t)A284389_zeros < n)
        A284389_grow();
    for (i = 0; i < A284389_len; ++i) {
        if (A284389_w[i] == 0) {
            ++cnt;
            if ((idx_t)cnt == n)
                return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
