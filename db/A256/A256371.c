/*
 * A256371  Inverse permutation to A256210.
 * https://oeis.org/A256371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in A256210 (inverse permutation); computed
 *            by generating A256210 (lexicographically earliest, a(1)=2,
 *            a(a(k)+a(k+1)) odd) until n appears
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9500 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A256371_perm = NULL;
static idx_t *A256371_inv = NULL;
static bool *A256371_used = NULL;
static bool *A256371_req = NULL;
static idx_t A256371_cap = 0;
static idx_t A256371_len = 0;
static idx_t A256371_lo = 1;

static void A256371_ensure(idx_t need)
{
    if (need <= A256371_cap) return;
    idx_t nc = A256371_cap ? A256371_cap : 16;
    while (nc < need) nc <<= 1;
    A256371_perm = mem_resize(seq_t, A256371_perm, (nc + 1));
    A256371_inv = (idx_t *)realloc(A256371_inv, (nc + 1) * sizeof(idx_t));
    A256371_used = (bool *)realloc(A256371_used, (nc + 1) * sizeof(bool));
    A256371_req = (bool *)realloc(A256371_req, (nc + 1) * sizeof(bool));
    for (idx_t i = A256371_cap ? A256371_cap + 1 : 0; i <= nc; ++i) {
        A256371_inv[i] = 0;
        A256371_used[i] = false;
        A256371_req[i] = false;
    }
    A256371_cap = nc;
}

static void A256371_init(void)
{
    if (A256371_perm != NULL) return;
    A256371_ensure(16);
    A256371_perm[1] = (seq_t)2;
    A256371_used[2] = true;
    A256371_inv[2] = 1;
    A256371_len = 1;
    A256371_lo = 1;
}

static void A256371_step(void)
{
    idx_t cur = A256371_len + 1;
    A256371_ensure(cur);
    bool needOdd = A256371_req[cur];
    while (A256371_used[A256371_lo]) ++A256371_lo;
    idx_t cand = A256371_lo;
    idx_t prev = (idx_t)(unsigned long long)A256371_perm[cur - 1];
    for (;; ++cand) {
        A256371_ensure(cand);
        if (A256371_used[cand]) continue;
        if (needOdd && ((cand & 1) == 0)) continue;
        idx_t S = prev + cand;
        if (S <= A256371_len) {
            if ((A256371_perm[S] & 1) == 0) continue;
        } else {
            A256371_ensure(S);
            A256371_req[S] = true;
        }
        break;
    }
    A256371_perm[cur] = (seq_t)cand;
    A256371_used[cand] = true;
    A256371_inv[cand] = cur;
    A256371_len = cur;
}

seq_t a(idx_t n)
{
    A256371_init();
    A256371_ensure(n);
    while (A256371_inv[n] == 0) {
        A256371_step();
        A256371_ensure(n);
    }
    return (seq_t)A256371_inv[n];
}
