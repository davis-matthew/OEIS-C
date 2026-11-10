/*
 * A373224  Row sums of A373223.
 * https://oeis.org/A373224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} T(n,k) (row sums of A373223); T(n,k) = -1
 *            iff prime(n), prime(k) both == 3 mod 4 and distinct, 0 if equal,
 *            else 1. So a(n) = n-1 - 2*#{k<n : prime(k) == 3 mod 4} if
 *            prime(n) == 3 mod 4, else n-1
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A373224_pl = NULL;
static size_t A373224_cnt = 0, A373224_cap = 0;

static void A373224_ensure(size_t need)
{
    uint32_t cand = (A373224_cnt == 0) ? 2u : A373224_pl[A373224_cnt - 1] + 1u;
    while (A373224_cnt < need) {
        bool isp = true;
        for (size_t i = 0; i < A373224_cnt; ++i) {
            uint64_t p = A373224_pl[i];
            if (p * p > cand) break;
            if (cand % p == 0) { isp = false; break; }
        }
        if (isp) {
            if (A373224_cnt == A373224_cap) {
                size_t nc = A373224_cap ? A373224_cap * 2 : 64;
                uint32_t *t = (uint32_t *)realloc(A373224_pl, nc * sizeof(uint32_t));
                if (!t) abort();
                A373224_pl = t;
                A373224_cap = nc;
            }
            A373224_pl[A373224_cnt++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A373224_ensure((size_t)n);
    seq_t res = (seq_t)(n - 1);
    if (A373224_pl[n - 1] % 4 == 3) {
        idx_t c = 0;
        for (idx_t k = 0; k + 1 < n; ++k)
            if (A373224_pl[k] % 4 == 3) ++c;
        res -= (seq_t)2 * (seq_t)c;
    }
    return res;
}
