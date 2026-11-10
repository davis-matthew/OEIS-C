/*
 * A325810  Lexicographically earliest sequence such that a(i) = a(j) =>
 *          A034460(i) = A034460(j) and A325814(i) = A325814(j) for all i, j.
 * https://oeis.org/A325810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of pairs [A034460(n),
 *            A325814(n)]: a(n) = index (by first occurrence) of the pair
 *            (usigma(n)-n, n-(sigma(n)-usigma(n))). Computed incrementally by
 *            trial-division factorization with memoized classes
 * Range:     u8 n <= 266, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 110 terms through n = 9534 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void A325810_pair(idx_t m, seq_t *x, seq_t *y)
{
    seq_t usig = 1, sig = 1;
    idx_t t = m;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            seq_t pw = 1, s = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)p;
                s += pw;
            }
            usig *= (pw + 1);
            sig *= s;
        }
    }
    if (t > 1) {
        usig *= ((seq_t)t + 1);
        sig *= ((seq_t)t + 1);
    }
    *x = usig - (seq_t)m;
    *y = (seq_t)m - (sig - usig);
}

static size_t *A325810_cls = 0;
static size_t A325810_cap = 0;
static idx_t A325810_done = 0;
static seq_t *A325810_k1 = 0;
static seq_t *A325810_k2 = 0;
static size_t A325810_nk = 0;
static size_t A325810_kcap = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((size_t)n + 1 > A325810_cap) {
        size_t nc = A325810_cap ? A325810_cap : 1024;
        while (nc < (size_t)n + 1)
            nc *= 2;
        A325810_cls = (size_t *)realloc(A325810_cls, nc * sizeof(size_t));
        A325810_cap = nc;
    }
    while (A325810_done < n) {
        idx_t m = A325810_done + 1;
        seq_t x, y;
        A325810_pair(m, &x, &y);
        size_t found = 0;
        for (size_t i = 0; i < A325810_nk; ++i) {
            if (A325810_k1[i] == x && A325810_k2[i] == y) {
                found = i + 1;
                break;
            }
        }
        if (!found) {
            if (A325810_nk == A325810_kcap) {
                size_t nk = A325810_kcap ? A325810_kcap * 2 : 1024;
                if (A325810_kcap == 0) {
                    A325810_k1 = mem_new(seq_t, nk);
                    A325810_k2 = mem_new(seq_t, nk);
                } else {
                    A325810_k1 = mem_resize(seq_t, A325810_k1, nk);
                    A325810_k2 = mem_resize(seq_t, A325810_k2, nk);
                }
                A325810_kcap = nk;
            }
            A325810_k1[A325810_nk] = x;
            A325810_k2[A325810_nk] = y;
            ++A325810_nk;
            found = A325810_nk;
        }
        A325810_cls[m] = found;
        A325810_done = m;
    }
    return (seq_t)A325810_cls[n];
}
