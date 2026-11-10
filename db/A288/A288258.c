/*
 * A288258  Positions of 0 in A288257; complement of A288259.
 * https://oeis.org/A288258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 0 in A288257, the fixed point of
 *            00->0010, 1->100 starting from 00 (computed by generating the
 *            word)
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9948 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A288258_bits = NULL;
static size_t A288258_len = 0;

static void A288258_ensure(size_t need)
{
    if (A288258_bits == NULL) {
        A288258_bits = (unsigned char *)calloc(2, 1);
        A288258_len = 2;
    }
    while (A288258_len < need) {
        size_t old_len = A288258_len;
        unsigned char *nb = (unsigned char *)calloc(old_len * 3 + 4, 1);
        size_t nl = 0, i = 0;
        while (i < old_len) {
            if (A288258_bits[i] == 0 && i + 1 < old_len && A288258_bits[i + 1] == 0) {
                nb[nl++] = 0; nb[nl++] = 0; nb[nl++] = 1; nb[nl++] = 0;
                i += 2;
            } else if (A288258_bits[i] == 1) {
                nb[nl++] = 1; nb[nl++] = 0; nb[nl++] = 0;
                i += 1;
            } else {
                nb[nl++] = 0;
                i += 1;
            }
        }
        free(A288258_bits);
        A288258_bits = nb;
        A288258_len = nl;
    }
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    A288258_ensure(2 * target + 8);
    size_t count = 0;
    for (size_t i = 0; i < A288258_len; i++) {
        if (A288258_bits[i] == 0) {
            count++;
            if (count == target) return (seq_t)(i + 1);
        }
    }
    return 0;
}
