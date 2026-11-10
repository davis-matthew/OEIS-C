/*
 * A283476  Squarefree numbers n for which A079559(A048675(n)) = 1.
 * https://oeis.org/A283476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   squarefree n such that m = A048675(n) is a term of A005187, i.e.
 *            m = 2k - popcount(k) for some k >= 0. Search: scan n upward; m's
 *            bit i-1 is set iff the i-th prime divides n, which is checked
 *            with a small-word popcount test
 * Range:     u8 n <= 94, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 89 terms through n = 8033 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool oeis_m_ok(uint64_t L, unsigned hp)
{
    /* m = L + 2^64 * (high part with hp set bits); m is in A005187 iff
       some c has (m+c) even, k=(m+c)/2, popcount(k)=c */
    for (unsigned c = 0; c < 64; ++c) {
        uint64_t t = L + c;
        if (t & 1) continue;
        if ((unsigned)__builtin_popcountll(t) + hp == c) return true;
    }
    return false;
}

static bool oeis_test(uint64_t n, const uint32_t *pr)
{
    uint64_t r = n, L = 0;
    unsigned hp = 0;
    for (int i = 0; i < 64; ++i) {
        uint64_t p = pr[i];
        if (r % p == 0) {
            r /= p;
            if (r % p == 0) return false;
            L |= (uint64_t)1 << i;
        }
    }
    for (uint64_t q = 313; q * q <= r; q += 2) {
        if (r % q == 0) {
            r /= q;
            if (r % q == 0) return false;
            ++hp;
        }
    }
    if (r > 1) ++hp;
    return oeis_m_ok(L, hp);
}

seq_t a(idx_t n)
{
    static uint32_t pr[64];
    static bool init = false;
    static uint64_t *res = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t cur = 0;
    if (!init) {
        int k = 0;
        for (uint32_t x = 2; k < 64; ++x) {
            bool ok = true;
            for (int j = 0; j < k && (uint64_t)pr[j] * pr[j] <= x; ++j)
                if (x % pr[j] == 0) { ok = false; break; }
            if (ok) pr[k++] = x;
        }
        init = true;
    }
    while (cnt < (size_t)n) {
        ++cur;
        if (oeis_test(cur, pr)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                res = (uint64_t *)realloc(res, cap * sizeof(uint64_t));
            }
            res[cnt++] = cur;
        }
    }
    return (seq_t)res[n - 1];
}
