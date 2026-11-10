/*
 * A328316  Iterates of A276086 starting from 0.
 * https://oeis.org/A328316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = A276086(a(n-1)) for n > 0, where A276086(m) =
 *            Product_{k>=0} prime(k+1)^d_k and d_k are the digits of m in
 *            primorial base
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 11 terms through n = 10 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

#define A328316_MAXK 4096

static uint32_t A328316_prime(int k)
{
    static uint32_t pr[A328316_MAXK + 8];
    static int cnt = 0;
    if (cnt == 0) { pr[0] = 2; cnt = 1; }
    while (cnt < k) {
        uint32_t c = pr[cnt - 1] + (cnt == 1 ? 1 : 2);
        for (;; c += 2) {
            bool ok = true;
            for (int i = 0; i < cnt; ++i) {
                uint32_t q = pr[i];
                if ((uint64_t)q * q > c) break;
                if (c % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        pr[cnt++] = c;
    }
    return pr[k - 1];
}

static seq_t A328316_step(seq_t m)
{
    if (m == 0) return (seq_t)1;
    seq_t *prim = mem_new(seq_t, A328316_MAXK);
    prim[0] = 1;
    int K = 0;
    while (K + 1 < A328316_MAXK) {
        seq_t p = (seq_t)A328316_prime(K + 1);
        seq_t nx = prim[K] * p;
        if (nx / p != prim[K]) break;   /* overflow in fixed width */
        if (nx > m) break;
        prim[++K] = nx;
    }
    seq_t result = 1;
    for (int k = K; k >= 0; --k) {
        seq_t d = m / prim[k];
        m = m % prim[k];
        long long dd = (d > (seq_t)1000) ? 1000 : (long long)d;
        seq_t p = (seq_t)A328316_prime(k + 1);
        for (long long j = 0; j < dd; ++j) result = result * p;
    }
    mem_free(prim);
    return result;
}

seq_t a(idx_t n)
{
    seq_t v = 0;
    for (idx_t i = 0; i < n; ++i) v = A328316_step(v);
    return v;
}
