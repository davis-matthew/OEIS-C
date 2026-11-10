/*
 * A328387  Numbers k such that A276086(k) is a multiple of k.
 * https://oeis.org/A328387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A term k is odd, and if p^e exactly divides k then e <=
 *            the primorial-base digit of k for p, so 0 < e <= p-1 and k is
 *            53-smooth (k < 2^64). For a bound B, list every such k <= B and
 *            keep those where A276086(k) mod k = 0. Raise B until at least n
 *            terms are found
 * Range:     u8 n <= 8, u16 n <= 92, u32 n <= 3003, u64 n <= 3003,
 *            u128 n <= 3003, big any n
 * Validated: standard, pass, 72 terms through n = 2855 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static const uint64_t A328387_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t A328387_prim[16];   /* prim[i] = product of first i primes */
static int A328387_exp[16];

static uint64_t *A328387_found = NULL;
static size_t A328387_fcnt = 0, A328387_fcap = 0;

static void A328387_push(uint64_t k)
{
    if (A328387_fcnt == A328387_fcap) {
        A328387_fcap = A328387_fcap ? A328387_fcap * 2 : 1024;
        A328387_found = (uint64_t *)realloc(A328387_found, A328387_fcap * sizeof(uint64_t));
        if (!A328387_found) abort();
    }
    A328387_found[A328387_fcnt++] = k;
}

/* check: exponent of prime i in k <= primorial digit i of k */
static bool A328387_check(uint64_t k)
{
    uint64_t rem = k;
    for (int i = 15; i >= 0; --i) {
        uint64_t d = rem / A328387_prim[i];
        rem -= d * A328387_prim[i];
        if ((uint64_t)A328387_exp[i] > d) return false;
    }
    return true;
}

static void A328387_dfs(int idx, uint64_t cur, uint64_t B)
{
    if (idx == 16) {
        if (A328387_check(cur)) A328387_push(cur);
        return;
    }
    uint64_t p = A328387_pr[idx];
    /* prime idx can only be used if k >= prim[idx] (else its digit is 0) */
    int maxe = (int)p - 1;
    if (A328387_prim[idx] > B) maxe = 0;
    uint64_t v = cur;
    for (int e = 0; e <= maxe; ++e) {
        A328387_exp[idx] = e;
        A328387_dfs(idx + 1, v, B);
        if (e == maxe) break;
        if (v > B / p) break;
        v *= p;
    }
    A328387_exp[idx] = 0;
}

static int A328387_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, w = *(const uint64_t *)y;
    return (u > w) - (u < w);
}

seq_t a(idx_t n)
{
    static bool init = false;
    static uint64_t bound = 0;
    if (!init) {
        A328387_prim[0] = 1;
        for (int i = 1; i < 16; ++i)
            A328387_prim[i] = A328387_prim[i - 1] * A328387_pr[i - 1];
        init = true;
    }
    if (n == 0) return (seq_t)0;
    if (bound == 0 || A328387_fcnt < (size_t)n) {
        uint64_t B = bound ? bound : ((uint64_t)1 << 16);
        while (1) {
            if (bound != 0) {
                if (B > UINT64_MAX / 16) {
                    if (B == UINT64_MAX) return (seq_t)0;
                    B = UINT64_MAX;
                } else {
                    B *= 16;
                }
            }
            A328387_fcnt = 0;
            for (int i = 0; i < 16; ++i) A328387_exp[i] = 0;
            /* skip prime 2: all terms are odd */
            A328387_dfs(1, 1, B);
            bound = B;
            if (A328387_fcnt >= (size_t)n) break;
        }
        qsort(A328387_found, A328387_fcnt, sizeof(uint64_t), A328387_cmp);
    }
    return (seq_t)A328387_found[(size_t)n - 1];
}
