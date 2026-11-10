/*
 * A323827  Apply Lenormand's "raboter" operation to A306211.
 * https://oeis.org/A323827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term of A306211 with each run of equal terms
 *            shortened by one term (Lenormand's "raboter"); computed directly
 *            from A306211's run structure
 * Range:     u8 n <= 16502, u16 n <= 16502, u32 n <= 16502, u64 n <= 16502,
 *            u128 n <= 16502, big any n
 * Validated: standard, pass, 103 terms through n = 9994 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A306211_seq = NULL;
static size_t A306211_len = 0;
static size_t A306211_cap = 0;

static void A306211_ensure(size_t need)
{
    if (need <= A306211_cap) return;
    size_t nc = A306211_cap ? A306211_cap * 2 : 16;
    while (nc < need) nc *= 2;
    A306211_seq = mem_resize(seq_t, A306211_seq, nc);
    A306211_cap = nc;
}

static void A306211_next(void)
{
    size_t run_cnt = 0;
    seq_t *runs = mem_new(seq_t, A306211_len);
    size_t run_len = 1;
    for (size_t i = 0; i + 1 < A306211_len; ++i) {
        if (A306211_seq[i] == A306211_seq[i + 1]) ++run_len;
        else { runs[run_cnt++] = (seq_t)run_len; run_len = 1; }
    }
    runs[run_cnt++] = (seq_t)run_len;
    A306211_ensure(A306211_len + run_cnt);
    for (size_t j = 0; j < run_cnt; ++j) A306211_seq[A306211_len + j] = runs[j];
    A306211_len += run_cnt;
    mem_free(runs);
}

static void A306211_grow(size_t n)
{
    if (A306211_seq == NULL) {
        A306211_cap = 16;
        A306211_seq = mem_new(seq_t, A306211_cap);
        A306211_len = 1;
        A306211_seq[0] = (seq_t)1;
    }
    while (A306211_len < n) A306211_next();
}

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    size_t L = 16;
    for (;;) {
        A306211_grow(L);
        size_t len = A306211_len;
        /* scan complete runs only (exclude the last, possibly incomplete run) */
        size_t count = 0;
        size_t i = 0;
        while (i < len) {
            size_t j = i;
            while (j + 1 < len && A306211_seq[j + 1] == A306211_seq[i]) ++j;
            if (j + 1 >= len) break;  /* last run: may be incomplete */
            size_t rl = j - i + 1;    /* run emits rl-1 terms */
            if (count + (rl - 1) >= want) {
                return A306211_seq[i];
            }
            count += rl - 1;
            i = j + 1;
        }
        L = len + 1;
    }
}
