/*
 * A330263  Distinct values of A308551 in order of their appearance as n
 *          grows.
 * https://oeis.org/A330263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th distinct value of A308551 in order of first
 *            appearance; computed by simulating the persistent stack of
 *            A308551 for n = 1, 2, 3, ... and recording each new top-of-stack
 *            value
 * Range:     u8 n <= 39, u16 n <= 4267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9278 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A330263_HSZ 65537

static seq_t *A330263_stack = NULL;
static size_t A330263_ssize = 0, A330263_scap = 0;

static seq_t *A330263_vals = NULL;
static int *A330263_next = NULL;
static size_t A330263_cnt = 0, A330263_vcap = 0;
static int A330263_head[A330263_HSZ];
static bool A330263_init = false;
static idx_t A330263_i = 0;

static void A330263_push(seq_t v) {
    if (A330263_ssize == A330263_scap) {
        size_t nc = A330263_scap ? A330263_scap * 2 : 16;
        seq_t *t = mem_resize(seq_t, A330263_stack, nc);
        if (!t) abort();
        A330263_stack = t;
        A330263_scap = nc;
    }
    A330263_stack[A330263_ssize++] = v;
}

static void A330263_record(seq_t v) {
    int h = (int)(v % (seq_t)A330263_HSZ);
    for (int k = A330263_head[h]; k >= 0; k = A330263_next[k]) {
        if (A330263_vals[k] == v) return;
    }
    if (A330263_cnt == A330263_vcap) {
        size_t nc = A330263_vcap ? A330263_vcap * 2 : 64;
        seq_t *t = mem_resize(seq_t, A330263_vals, nc);
        if (!t) abort();
        A330263_vals = t;
        int *tn = (int *)realloc(A330263_next, nc * sizeof(int));
        if (!tn) abort();
        A330263_next = tn;
        A330263_vcap = nc;
    }
    A330263_vals[A330263_cnt] = v;
    A330263_next[A330263_cnt] = A330263_head[h];
    A330263_head[h] = (int)A330263_cnt;
    A330263_cnt++;
}

seq_t a(idx_t n) {
    if (!A330263_init) {
        for (int k = 0; k < A330263_HSZ; k++) A330263_head[k] = -1;
        A330263_init = true;
    }
    while ((idx_t)A330263_cnt < n) {
        idx_t i = ++A330263_i;
        int msb = 0;
        idx_t t = i;
        while (t >>= 1) ++msb;
        for (int pos = msb; pos >= 0; --pos) {
            if ((i >> pos) & 1) {
                A330263_push((seq_t)1);
            } else {
                seq_t b = A330263_stack[--A330263_ssize];
                seq_t c = A330263_stack[--A330263_ssize];
                A330263_push(c + b);
            }
        }
        A330263_record(A330263_stack[A330263_ssize - 1]);
    }
    return A330263_vals[n - 1];
}
