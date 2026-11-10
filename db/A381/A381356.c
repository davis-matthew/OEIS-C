/*
 * A381356  Limit of rows in irregular triangle A381587.
 * https://oeis.org/A381356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term of the RUNS transform of A306346, where A306346
 *            is the limit of S(1)=[0], S(k+1)=S(k) concatenated with the RUNS
 *            transform of reverse(S(k))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 114 terms through n = 4942 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A381356_S = NULL;
static size_t A381356_slen = 0, A381356_scap = 0;
static seq_t *A381356_R = NULL;
static size_t A381356_rlen = 0, A381356_rcap = 0;

static void A381356_push_S(seq_t v)
{
    if (A381356_slen >= A381356_scap) {
        size_t nc = A381356_scap ? A381356_scap * 2 : 16;
        A381356_S = mem_resize(seq_t, A381356_S, nc);
        A381356_scap = nc;
    }
    A381356_S[A381356_slen++] = v;
}

static void A381356_push_R(seq_t v)
{
    if (A381356_rlen >= A381356_rcap) {
        size_t nc = A381356_rcap ? A381356_rcap * 2 : 16;
        A381356_R = mem_resize(seq_t, A381356_R, nc);
        A381356_rcap = nc;
    }
    A381356_R[A381356_rlen++] = v;
}

static void A381356_extend(void)
{
    if (A381356_slen == 0) {
        A381356_push_S((seq_t)0);
        return;
    }
    size_t old = A381356_slen;
    size_t i = old;
    while (i > 0) {
        seq_t val = A381356_S[i - 1];
        seq_t cnt = 1;
        i--;
        while (i > 0 && A381356_S[i - 1] == val) {
            cnt++;
            i--;
        }
        A381356_push_S(cnt);
    }
}

static void A381356_recompute_runs(void)
{
    A381356_rlen = 0;
    size_t i = 0;
    while (i < A381356_slen) {
        size_t j = i;
        while (j < A381356_slen && A381356_S[j] == A381356_S[i]) j++;
        if (j >= A381356_slen) break; /* last run may be incomplete */
        A381356_push_R((seq_t)(j - i));
        i = j;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A381356_slen == 0) A381356_extend();
    while (A381356_rlen < (size_t)n) {
        A381356_extend();
        A381356_recompute_runs();
    }
    return A381356_R[(size_t)n - 1];
}
