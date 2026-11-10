/*
 * A364963  Odd numbers k such that k is a multiple of A163511(k).
 * https://oeis.org/A364963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search, not a closed form. a(n) is the n-th smallest odd k with
 *            A163511(k) | k. A depth-first search builds k bit by bit,
 *            starting from the lowest bit. Here A163511(k) = p(r) * Prod_i
 *            p(i)^z_i, where r is the number of 1 bits in k and z_i is the
 *            number of zeros just below the i-th 1 bit. A branch...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 A364963_u128;

static uint64_t A364963_pr[80];
static int A364963_pr_init = 0;

static uint64_t *A364963_res = NULL;
static size_t A364963_rc = 0, A364963_rcap = 0;
static int A364963_doneB = 0;

static void A364963_init_primes(void)
{
    int cnt = 0;
    uint64_t x = 2;
    if (A364963_pr_init) return;
    while (cnt < 80) {
        int ok = 1;
        int i;
        for (i = 0; i < cnt && A364963_pr[i] * A364963_pr[i] <= x; ++i)
            if (x % A364963_pr[i] == 0) { ok = 0; break; }
        if (ok) A364963_pr[cnt++] = x;
        ++x;
    }
    A364963_pr_init = 1;
}

static void A364963_push(uint64_t v)
{
    if (A364963_rc == A364963_rcap) {
        size_t nc = A364963_rcap ? A364963_rcap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A364963_res, nc * sizeof(uint64_t));
        if (!t) return;
        A364963_res = t;
        A364963_rcap = nc;
    }
    A364963_res[A364963_rc++] = v;
}

/* kv: current k with c ones, top one at bit L-1; P: product for completed gaps */
static void A364963_dfs(uint64_t kv, int L, int c, A364963_u128 P,
                        int B, A364963_u128 lim)
{
    A364963_u128 m = P * A364963_pr[c - 1];
    int z;
    A364963_u128 P2;
    if (m <= kv && kv % (uint64_t)m == 0) A364963_push(kv);
    P2 = P;
    for (z = 0;; ++z) {
        int L2 = L + z + 1;
        if (L2 > B) break;
        if (P2 * A364963_pr[c] >= lim) break;
        A364963_dfs(kv | ((uint64_t)1 << (L + z)), L2, c + 1, P2, B, lim);
        P2 *= A364963_pr[c];
    }
}

static int A364963_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return (a > b) - (a < b);
}

static void A364963_search(int B)
{
    A364963_u128 lim = ((A364963_u128)1) << B;
    A364963_rc = 0;
    A364963_dfs(1, 1, 1, (A364963_u128)1, B, lim);
    qsort(A364963_res, A364963_rc, sizeof(uint64_t), A364963_cmp);
    A364963_doneB = B;
}

seq_t a(idx_t n)
{
    static const int steps[5] = {32, 40, 48, 56, 63};
    int s;
    if (n < 1) return (seq_t)0;
    A364963_init_primes();
    if (A364963_doneB && A364963_rc >= n) return (seq_t)A364963_res[n - 1];
    for (s = 0; s < 5; ++s) {
        if (steps[s] <= A364963_doneB) continue;
        A364963_search(steps[s]);
        if (A364963_rc >= n) return (seq_t)A364963_res[n - 1];
    }
    return (seq_t)0;
}
