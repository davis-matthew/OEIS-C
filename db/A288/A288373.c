/*
 * A288373  Positions of 0 in A288372; complement of A288316.
 * https://oeis.org/A288373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288372 (fixed point of
 *            00->0001, 1->01 starting with 00)
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9300 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static unsigned char *A288373_buf = NULL;
static size_t A288373_len = 0;

static void A288373_expand(void)
{
    size_t i = 0, nl = 0, p = 0;
    while (i < A288373_len) {
        if (A288373_buf[i] == 0 && i + 1 < A288373_len && A288373_buf[i + 1] == 0) {
            nl += 4; i += 2;
        } else {
            nl += (A288373_buf[i] == 1) ? 2 : 1; i += 1;
        }
    }
    unsigned char *nb = (unsigned char *)malloc(nl);
    i = 0;
    while (i < A288373_len) {
        if (A288373_buf[i] == 0 && i + 1 < A288373_len && A288373_buf[i + 1] == 0) {
            nb[p++] = 0; nb[p++] = 0; nb[p++] = 0; nb[p++] = 1; i += 2;
        } else {
            if (A288373_buf[i] == 1) { nb[p++] = 0; nb[p++] = 1; }
            else nb[p++] = 0;
            i += 1;
        }
    }
    free(A288373_buf);
    A288373_buf = nb;
    A288373_len = nl;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A288373_buf == NULL) {
        A288373_buf = (unsigned char *)malloc(2);
        A288373_buf[0] = 0;
        A288373_buf[1] = 0;
        A288373_len = 2;
    }
    for (;;) {
        if (A288373_len > 8) {
            size_t lim = A288373_len - 4;
            size_t cnt = 0;
            for (size_t i = 0; i < lim; i++) {
                if (A288373_buf[i] == 0) {
                    cnt++;
                    if ((idx_t)cnt == n) return (seq_t)(i + 1);
                }
            }
        }
        A288373_expand();
    }
}
