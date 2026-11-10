/*
 * A378743  a(n) is the least m for which bigomega(A246281(m)) = n.
 * https://oeis.org/A378743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = least k in A246281 (A003961(k) < 2k) with bigomega(k) =
 *            n; computed by bounded DFS over prime factorizations with
 *            iteratively enlarged limit. a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-10)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static uint64_t *A378743_pr = NULL;
static size_t A378743_pn = 0, A378743_pc = 0;

static uint64_t A378743_prime(size_t i)
{
    while (A378743_pn <= i) {
        if (A378743_pn == A378743_pc) {
            A378743_pc = A378743_pc ? A378743_pc * 2 : 256;
            A378743_pr = (uint64_t *)realloc(A378743_pr, A378743_pc * sizeof(uint64_t));
            if (!A378743_pr) abort();
        }
        if (A378743_pn == 0) A378743_pr[0] = 2;
        else A378743_pr[A378743_pn] = next_prime(A378743_pr[A378743_pn - 1]);
        A378743_pn++;
    }
    return A378743_pr[i];
}

static seq_t A378743_best;
static bool A378743_found;

static void A378743_dfs(size_t pi, int r, seq_t k, seq_t num)
{
    for (size_t i = pi;; i++) {
        seq_t p = (seq_t)A378743_prime(i);
        seq_t q = (seq_t)A378743_prime(i + 1);
        /* lower bound k * p^r <= best */
        {
            seq_t t = k;
            bool ok = true;
            for (int j = 0; j < r; j++) {
                if (t > A378743_best / p) { ok = false; break; }
                t *= p;
            }
            if (!ok) break;
        }
        seq_t k2 = k, n2 = num;
        for (int e = 1; e <= r; e++) {
            if (k2 > A378743_best / p) break;
            if (n2 > (A378743_best * 2) / q) break;
            k2 *= p;
            n2 *= q;
            if (n2 >= k2 * 2) break;
            if (e == r) {
                if (k2 <= A378743_best) { A378743_best = k2; A378743_found = true; }
            } else {
                A378743_dfs(i + 1, r - e, k2, n2);
            }
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t cache[128];
    static bool have[128];
    if (n == 0) return 1;
    if (n < 128 && have[n]) return cache[n];
    seq_t L = 1;
    for (idx_t i = 0; i < n; i++) L *= 2;
    seq_t result = 0;
    for (;;) {
        A378743_best = L;
        A378743_found = false;
        A378743_dfs(0, (int)n, 1, 1);
        if (A378743_found) { result = A378743_best; break; }
        seq_t L2 = L * 4;
        if (L2 / 4 != L || L2 < L) break;
        L = L2;
    }
    if (n < 128 && result != 0) { cache[n] = result; have[n] = true; }
    return result;
}
