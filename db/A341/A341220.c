/*
 * A341220  Irregular table read by rows T(n, k), n >= 0, k = 1..A341218(n);
 *          T(n, k) is the k-th number m such that A340873(m) = n.
 * https://oeis.org/A341220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base, tabf
 * Formula:   rows of the table list, in increasing order, all m with
 *            A340873(m) = n. Row n+1 is built from row n: for each m in row
 *            n, 2*m is in row n+1, and if m is even and > 2, the odd x =
 *            inverse Gray code of m/2 is in row n+1 when x is odd and > 1.
 *            The rows are generated one at a time, sorted and concate...
 * Range:     u8 n <= 25, u16 n <= 508, u32 n <= 11792, u64 n <= 11792,
 *            u128 n <= 11792, big any n
 * Validated: standard, pass, 89 terms through n = 9858 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A341220_flat = 0;
static size_t A341220_fcnt = 0, A341220_fcap = 0;
static seq_t *A341220_cur = 0;
static size_t A341220_ccnt = 0;

static void A341220_append_row(const seq_t *row, size_t cnt)
{
    if (A341220_fcnt + cnt > A341220_fcap) {
        size_t ncap = A341220_fcap ? A341220_fcap * 2 : 64;
        while (ncap < A341220_fcnt + cnt) ncap *= 2;
        seq_t *nf = mem_new(seq_t, ncap);
        for (size_t i = 0; i < A341220_fcnt; i++) nf[i] = A341220_flat[i];
        if (A341220_flat) mem_free(A341220_flat);
        A341220_flat = nf;
        A341220_fcap = ncap;
    }
    for (size_t i = 0; i < cnt; i++) A341220_flat[A341220_fcnt + i] = row[i];
    A341220_fcnt += cnt;
}

static void A341220_next_row(void)
{
    seq_t *nxt = mem_new(seq_t, 2 * A341220_ccnt + 2);
    size_t k = 0;
    for (size_t i = 0; i < A341220_ccnt; i++) {
        seq_t m = A341220_cur[i];
        nxt[k++] = m * 2;
        if ((m & 1) == 0 && m > 2) {
            seq_t g = m >> 1;
            seq_t x = g;
            seq_t t = g >> 1;
            while (t > 0) {
                x ^= t;
                t >>= 1;
            }
            if ((x & 1) == 1 && x > 1) nxt[k++] = x;
        }
    }
    /* insertion sort */
    for (size_t i = 1; i < k; i++) {
        seq_t v = nxt[i];
        size_t j = i;
        while (j > 0 && nxt[j - 1] > v) {
            nxt[j] = nxt[j - 1];
            j--;
        }
        nxt[j] = v;
    }
    mem_free(A341220_cur);
    A341220_cur = nxt;
    A341220_ccnt = k;
    A341220_append_row(nxt, k);
}

seq_t a(idx_t n)
{
    if (!A341220_cur) {
        A341220_cur = mem_new(seq_t, 1);
        A341220_cur[0] = 1;
        A341220_ccnt = 1;
        A341220_append_row(A341220_cur, 1);
    }
    while (A341220_fcnt <= (size_t)n) A341220_next_row();
    return A341220_flat[(size_t)n];
}
