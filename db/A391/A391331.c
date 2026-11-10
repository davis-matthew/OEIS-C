/*
 * A391331  Odd semiprimes k = p*q such that k = A325820(p,q), with p, q
 *          primes > 3, and A325820 is the carryless base-3 multiplication.
 * https://oeis.org/A391331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. For each prime p >= 5, a depth-first search over the
 *            base-3 digits of q >= p keeps only q whose digit convolution
 *            with p never reaches 3, so the carryless product equals p*q.
 *            Each such prime q adds p*q. The bound is raised until there are
 *            n terms, which are then sorted
 * Range:     u8 none, u16 n <= 94, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9080 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A391331_arr = 0;
static size_t A391331_cnt = 0;
static size_t A391331_cap = 0;
static uint64_t A391331_lim = 0;

static unsigned A391331_dp[48];
static unsigned A391331_lp;
static uint64_t A391331_p, A391331_qmin, A391331_qmax;
static uint64_t A391331_pw[41];
static unsigned A391331_c[100];

static int A391331_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A391331_push(uint64_t v)
{
    if (A391331_cnt == A391331_cap) {
        A391331_cap = A391331_cap ? A391331_cap * 2 : 1024;
        A391331_arr = (uint64_t *)realloc(A391331_arr, A391331_cap * sizeof(uint64_t));
    }
    A391331_arr[A391331_cnt++] = v;
}

static void A391331_dfs(int j, uint64_t pref)
{
    if (j < 0) {
        uint64_t q = pref;
        if (q < A391331_qmin || q > A391331_qmax) return;
        if ((q & 1) == 0 || q % 3 == 0) return;
        if (!is_prime_u64(q)) return;
        A391331_push(A391331_p * q);
        return;
    }
    uint64_t pw = A391331_pw[j];
    for (unsigned d = 0; d < 3; d++) {
        uint64_t base = pref + (uint64_t)d * pw;
        if (base > A391331_qmax) break;
        if (base + (pw - 1) < A391331_qmin) continue;
        if (j == 0 && d == 0) continue;
        if (d > 0) {
            unsigned i;
            bool ok = true;
            for (i = 0; i < A391331_lp; i++) {
                A391331_c[i + j] += d * A391331_dp[i];
                if (A391331_c[i + j] >= 3) { ok = false; i++; break; }
            }
            if (!ok) {
                for (unsigned k = 0; k < i; k++) A391331_c[k + j] -= d * A391331_dp[k];
                break; /* larger d only increases coefficients */
            }
            A391331_dfs(j - 1, base);
            for (unsigned k = 0; k < A391331_lp; k++) A391331_c[k + j] -= d * A391331_dp[k];
        } else {
            A391331_dfs(j - 1, base);
        }
    }
}

static void A391331_build(uint64_t L)
{
    A391331_cnt = 0;
    A391331_pw[0] = 1;
    for (int i = 1; i <= 40; i++) A391331_pw[i] = A391331_pw[i - 1] * 3;
    for (uint64_t p = 5; p * p <= L; p += 2) {
        if (p % 3 == 0) continue;
        if (!is_prime_u64(p)) continue;
        A391331_lp = 0;
        for (uint64_t t = p; t; t /= 3) A391331_dp[A391331_lp++] = (unsigned)(t % 3);
        A391331_p = p;
        A391331_qmin = p;
        A391331_qmax = L / p;
        int m = 0;
        while (m < 40 && A391331_pw[m] <= A391331_qmax) m++;
        for (int i = 0; i < 100; i++) A391331_c[i] = 0;
        A391331_dfs(m - 1, 0);
    }
    if (A391331_cnt > 1) qsort(A391331_arr, A391331_cnt, sizeof(uint64_t), A391331_cmp);
    A391331_lim = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A391331_lim == 0) A391331_build(65536);
    while (A391331_cnt < (size_t)n) A391331_build(A391331_lim * 4);
    return (seq_t)A391331_arr[n - 1];
}
