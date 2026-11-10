/*
 * A382710  Smallest missing number after A019444(n) has been computed.
 * https://oeis.org/A382710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{A019444(k) : 1<=k<=n}, where A019444 is the greedy
 *            sequence with integer initial averages (computed by simulation;
 *            s = running candidate average)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static size_t A382710_k = 0;          /* terms of A019444 processed */
static seq_t A382710_s = 1;           /* current candidate average */
static seq_t A382710_mex = 1;         /* current mex */
static seq_t *A382710_m = NULL;       /* A382710_m[n] = mex after n terms */
static size_t A382710_mcap = 0;
static unsigned char *A382710_used = NULL;
static size_t A382710_ucap = 0;

static void A382710_ensure_used(size_t idx)
{
    if (idx < A382710_ucap) return;
    size_t nc = A382710_ucap ? A382710_ucap : 16;
    while (nc <= idx) nc <<= 1;
    unsigned char *t = (unsigned char *)realloc(A382710_used, nc);
    if (!t) abort();
    for (size_t i = A382710_ucap; i < nc; ++i) t[i] = 0;
    A382710_used = t;
    A382710_ucap = nc;
}

seq_t a(idx_t n)
{
    if (n == 0) abort();
    size_t target = (size_t)n;
    if (target > A382710_k) {
        if (target + 1 > A382710_mcap) {
            size_t nc = target + 1;
            seq_t *t = mem_resize(seq_t, A382710_m, nc);
            if (!t) abort();
            A382710_m = t;
            A382710_mcap = nc;
        }
        while (A382710_k < target) {
            size_t next = A382710_k + 1;
            A382710_ensure_used((size_t)(long long)A382710_s + 2);
            seq_t cand;
            if (A382710_used[(size_t)(long long)A382710_s]) {
                cand = A382710_s + (seq_t)next;
                A382710_s = A382710_s + 1;
            } else {
                cand = A382710_s;
            }
            A382710_ensure_used((size_t)(long long)cand + 2);
            A382710_used[(size_t)(long long)cand] = 1;
            while (A382710_used[(size_t)(long long)A382710_mex]) {
                A382710_mex = A382710_mex + 1;
                A382710_ensure_used((size_t)(long long)A382710_mex + 1);
            }
            A382710_m[next] = A382710_mex;
            A382710_k = next;
        }
    }
    return A382710_m[target];
}
