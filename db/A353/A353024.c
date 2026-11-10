/*
 * A353024  Largest k such that A007504(k) <= n^2.
 * https://oeis.org/A353024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that Sum_{i=1..k} prime(i) <= n^2;
 *            computed by summing primes from a cached list until the sum
 *            exceeds n^2
 * Range:     u8 n <= 15, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *A353024_pr = NULL;
static seq_t *A353024_cum = NULL;
static size_t A353024_cnt = 0;
static size_t A353024_cap = 0;

static void A353024_add(void)
{
    uint64_t cand = A353024_cnt ? A353024_pr[A353024_cnt - 1] + 1 : 2;
    for (;; ++cand) {
        bool ok = true;
        for (size_t i = 0; i < A353024_cnt; ++i) {
            uint64_t p = A353024_pr[i];
            if (p > cand / p) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) break;
    }
    if (A353024_cnt == A353024_cap) {
        size_t nc = A353024_cap ? A353024_cap << 1 : 16;
        uint64_t *np = (uint64_t *)realloc(A353024_pr, nc * sizeof(uint64_t));
        if (!np) abort();
        A353024_pr = np;
        A353024_cum = mem_resize(seq_t, A353024_cum, nc);
        A353024_cap = nc;
    }
    A353024_pr[A353024_cnt] = cand;
    if (A353024_cnt == 0) A353024_cum[0] = (seq_t)cand;
    else A353024_cum[A353024_cnt] = A353024_cum[A353024_cnt - 1] + (seq_t)cand;
    ++A353024_cnt;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * (seq_t)n;
    while (A353024_cnt == 0 || A353024_cum[A353024_cnt - 1] <= target)
        A353024_add();
    /* cum[cnt-1] > target; find largest k with cum[k-1] <= target */
    size_t lo = 0, hi = A353024_cnt - 1; /* answer in [0, cnt-1] */
    while (lo < hi) {
        size_t mid = lo + (hi - lo + 1) / 2;
        if (A353024_cum[mid - 1] <= target) lo = mid;
        else hi = mid - 1;
    }
    return (seq_t)lo;
}
