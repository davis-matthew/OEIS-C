/*
 * A288379  Positions of 1 in A288377; complement of A288378.
 * https://oeis.org/A288379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288377, the fixed point of
 *            00->0001, 1->10 starting with 00; computed by iterating the
 *            rewriting and scanning
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 10000 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A288379_buf = NULL;
static idx_t A288379_len = 0;

static void A288379_extend(void)
{
    idx_t new_cap = A288379_len * 2 + 4;
    seq_t *nb = mem_new(seq_t, new_cap);
    idx_t pos = 0;
    for (idx_t i = 0; i < A288379_len; ) {
        if (A288379_buf[i] == (seq_t)0 && i + 1 < A288379_len && A288379_buf[i + 1] == (seq_t)0) {
            nb[pos++] = (seq_t)0;
            nb[pos++] = (seq_t)0;
            nb[pos++] = (seq_t)0;
            nb[pos++] = (seq_t)1;
            i += 2;
        } else if (A288379_buf[i] == (seq_t)1) {
            nb[pos++] = (seq_t)1;
            nb[pos++] = (seq_t)0;
            i += 1;
        } else {
            nb[pos++] = (seq_t)0;
            i += 1;
        }
    }
    mem_free(A288379_buf);
    A288379_buf = nb;
    A288379_len = pos;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A288379_buf == NULL) {
        A288379_buf = mem_new(seq_t, 4);
        A288379_buf[0] = (seq_t)0;
        A288379_buf[1] = (seq_t)0;
        A288379_len = 2;
    }
    for (;;) {
        /* count ones within the reliable first half */
        idx_t half = A288379_len / 2;
        idx_t cnt = 0;
        for (idx_t i = 0; i < half; i++) {
            if (A288379_buf[i] == (seq_t)1) {
                cnt++;
                if (cnt == n) return (seq_t)(i + 1);
            }
        }
        A288379_extend();
    }
}
