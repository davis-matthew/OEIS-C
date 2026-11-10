/*
 * A369066  Dirichlet convolution of Liouville's lambda (A008836) with
 *          A083345, as reordered by the Doudna sequence.
 * https://oeis.org/A369066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = A369069(A005940(1+n)), where A369069(m) = Sum_{d|m}
 *            lambda(m/d) * d'/gcd(d,d'), d' = arithmetic derivative of d
 * Range:     i8 n <= 43, i16 n <= 1738, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9624 (2026-11-10)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static uint64_t D_pf[64];
static uint64_t D_ex[64];
static uint64_t D_cur[64];
static size_t D_cnt;
static size_t D_suma;

static void D_dfs(size_t idx, seq_t d, size_t sum_b, seq_t *accum)
{
    if (idx == D_cnt) {
        seq_t dprime = 0;
        for (size_t j = 0; j < D_cnt; ++j) {
            uint64_t bj = D_cur[j];
            if (!bj) continue;
            seq_t term = d / (seq_t)D_pf[j];
            term *= (seq_t)bj;
            dprime += term;
        }
        seq_t g = gcd(d, dprime);
        seq_t ratio = dprime / g;
        if (((D_suma - sum_b) & 1) != 0) *accum -= ratio;
        else *accum += ratio;
        return;
    }
    seq_t d_val = d;
    for (uint64_t e = 0; e <= D_ex[idx]; ++e) {
        D_cur[idx] = e;
        D_dfs(idx + 1, d_val, sum_b + (size_t)e, accum);
        d_val *= (seq_t)D_pf[idx];
    }
}

static seq_t A369069_a(uint64_t m)
{
    if (m < 1) return 0;
    factor_u64(m, D_pf, D_ex, &D_cnt);
    D_suma = 0;
    for (size_t i = 0; i < D_cnt; ++i) { D_suma += (size_t)D_ex[i]; D_cur[i] = 0; }
    seq_t result = 0;
    D_dfs(0, (seq_t)1, 0, &result);
    return result;
}

static uint64_t A005940_a(uint64_t n)
{
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
    };
    uint64_t m = n - 1;
    uint64_t result = 1;
    size_t i = 1;
    while (m > 0) {
        if (m & 1) {
            if (i - 1 < 20) result *= primes[i - 1];
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t m = A005940_a((uint64_t)n + 1);
    return A369069_a(m);
}
