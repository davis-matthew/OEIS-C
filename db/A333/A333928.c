/*
 * A333928  Recursive abundant numbers: numbers k such that A333926(k) > 2*k.
 * https://oeis.org/A333928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A333926(k) > 2*k, where A333926 is the
 *            multiplicative sum of recursive divisors, with A333926(p^e) = 1
 *            + Sum_{d recursive divisor of e} p^d. Computed by a cached
 *            incremental search
 * Range:     u8 n <= 16, u16 n <= 3617, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 7748 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define RD_MAX 70

static bool rd_marks[RD_MAX + 1][RD_MAX + 1];
static bool rd_done[RD_MAX + 1];

static void rd_compute(int k)
{
    if (k > RD_MAX || rd_done[k]) return;
    for (int i = 1; i < k; ++i) rd_compute(i);
    for (int d = 1; d <= k; ++d) rd_marks[k][d] = false;
    for (int i = 1; i < k; ++i)
        for (int d = 1; d <= i; ++d)
            if (rd_marks[i][d] && (k % d == 0)) rd_marks[k][d] = true;
    rd_marks[k][k] = true;
    rd_done[k] = true;
}

static seq_t A333928_rsigma(uint64_t n)
{
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= n || n > 1; ++p) {
        if (p * p > n) p = n;
        if (n % p) continue;
        int e = 0;
        while (n % p == 0) { n /= p; ++e; }
        rd_compute(e);
        seq_t sum = 1;
        seq_t pw = 1;
        for (int d = 1; d <= e; ++d) {
            pw *= (seq_t)p;
            if (rd_marks[e][d]) sum += pw;
        }
        result *= sum;
    }
    return result;
}

static uint64_t *terms_buf = NULL;
static size_t terms_cnt = 0, terms_cap = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    while (terms_cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (k < 2) continue;
        seq_t s = A333928_rsigma(k);
        if (s > (seq_t)2 * (seq_t)k) {
            if (terms_cnt == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 1024;
                terms_buf = (uint64_t *)realloc(terms_buf, terms_cap * sizeof(uint64_t));
            }
            terms_buf[terms_cnt++] = k;
        }
    }
    return (seq_t)terms_buf[n - 1];
}
