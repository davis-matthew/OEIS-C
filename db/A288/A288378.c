/*
 * A288378  Positions of 0 in A288377; complement of A288379.
 * https://oeis.org/A288378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288377, the fixed point of
 *            00->0001, 1->10 starting with 00 (computed by iterating the
 *            substitution)
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9449 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static unsigned char *A288378_buf = NULL;
static size_t A288378_len = 0;
static size_t A288378_zeros = 0;

static void A288378_step(void)
{
    unsigned char *nb = (unsigned char *)malloc(A288378_len * 2 + 8);
    size_t pos = 0, i = 0;
    while (i < A288378_len) {
        if (A288378_buf[i] == 0 && i + 1 < A288378_len && A288378_buf[i + 1] == 0) {
            nb[pos++] = 0; nb[pos++] = 0; nb[pos++] = 0; nb[pos++] = 1;
            i += 2;
        } else if (A288378_buf[i] == 1) {
            nb[pos++] = 1; nb[pos++] = 0;
            i += 1;
        } else {
            nb[pos++] = 0;
            i += 1;
        }
    }
    free(A288378_buf);
    A288378_buf = nb;
    A288378_len = pos;
    size_t z = 0;
    for (size_t k = 0; k < pos; k++) if (nb[k] == 0) z++;
    A288378_zeros = z;
}

seq_t a(idx_t n)
{
    if (A288378_buf == NULL) {
        A288378_buf = (unsigned char *)malloc(8);
        A288378_buf[0] = 0;
        A288378_buf[1] = 0;
        A288378_len = 2;
        A288378_zeros = 2;
    }
    while ((idx_t)A288378_zeros < n) A288378_step();
    idx_t cnt = 0;
    for (size_t i = 0; i < A288378_len; i++) {
        if (A288378_buf[i] == 0) {
            cnt++;
            if (cnt == n) return (seq_t)(i + 1);
        }
    }
    return (seq_t)0;
}
