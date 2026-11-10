/*
 * A323826  RUNS transform of A306211.
 * https://oeis.org/A323826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of consecutive equal terms in
 *            A306211, where A306211 starts [1] and repeatedly appends the run
 *            lengths of the current sequence
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A323826_s = NULL;
static size_t A323826_len = 0;
static size_t A323826_cap = 0;

static void A323826_ensure(size_t need)
{
    if (need <= A323826_cap) return;
    size_t nc = A323826_cap ? A323826_cap * 2 : 16;
    while (nc < need) nc *= 2;
    A323826_s = mem_resize(seq_t, A323826_s, nc);
    A323826_cap = nc;
}

static void A323826_grow(void)
{
    size_t cnt = 0;
    seq_t *runs = mem_new(seq_t, A323826_len);
    size_t run_len = 1;
    for (size_t i = 0; i + 1 < A323826_len; ++i) {
        if (A323826_s[i] == A323826_s[i + 1]) {
            ++run_len;
        } else {
            runs[cnt++] = (seq_t)run_len;
            run_len = 1;
        }
    }
    runs[cnt++] = (seq_t)run_len;
    A323826_ensure(A323826_len + cnt);
    for (size_t j = 0; j < cnt; ++j) A323826_s[A323826_len + j] = runs[j];
    A323826_len += cnt;
    mem_free(runs);
}

static size_t A323826_changes(void)
{
    size_t c = 0;
    for (size_t i = 0; i + 1 < A323826_len; ++i)
        if (A323826_s[i] != A323826_s[i + 1]) ++c;
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A323826_s == NULL) {
        A323826_cap = 16;
        A323826_s = mem_new(seq_t, A323826_cap);
        A323826_len = 1;
        A323826_s[0] = (seq_t)1;
    }
    /* need n completed runs: n changes means n runs are finalized */
    while ((idx_t)A323826_changes() < n) A323826_grow();

    idx_t k = 0;
    size_t run_len = 1;
    for (size_t i = 0; i + 1 < A323826_len; ++i) {
        if (A323826_s[i] == A323826_s[i + 1]) {
            ++run_len;
        } else {
            ++k;
            if (k == n) return (seq_t)run_len;
            run_len = 1;
        }
    }
    return (seq_t)run_len;
}
