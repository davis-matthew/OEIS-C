/*
 * A376889  Numbers k such that A376888(k) = 2*k.
 * https://oeis.org/A376889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = n-th k with A376888(k) = 2*k, found by a DFS over
 *            factorizations (pruned by an abundancy bound) with a closed form
 *            for the last prime; the search limit grows by 10x until n terms
 *            are found
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned __int128 A376889_u128;

static uint64_t *A376889_pr = 0;
static size_t A376889_np = 0;
static uint64_t A376889_L = 0;
static uint64_t A376889_res[4096];
static size_t A376889_cnt = 0;
static uint64_t A376889_done_limit = 0;

static bool A376889_isprime(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t sp[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < 12; i++) {
        if (n == sp[i]) return true;
        if (n % sp[i] == 0) return false;
    }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t a = sp[i];
        uint64_t x = 1, b = a % n, e = d;
        while (e) {
            if (e & 1) x = (uint64_t)((A376889_u128)x * b % n);
            b = (uint64_t)((A376889_u128)b * b % n);
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = (uint64_t)((A376889_u128)x * x % n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* A376888 on a prime power q^e */
static uint64_t A376889_fpp(uint64_t q, uint64_t e)
{
    uint64_t total = 1, rem = e, k = 1, fact = 1;
    while (rem > 0) {
        uint64_t d = rem % (k + 1);
        rem /= (k + 1);
        if (d > 0) {
            uint64_t qf = 1;
            for (uint64_t i = 0; i < fact; i++) qf *= q;
            uint64_t cur = 1, t = 1;
            for (uint64_t i = 1; i <= d; i++) { cur *= qf; t += cur; }
            total *= t;
        }
        k++;
        fact *= k;
    }
    return total;
}

static void A376889_record(uint64_t v)
{
    if (A376889_cnt < 4096) A376889_res[A376889_cnt++] = v;
}

static void A376889_dfs(uint64_t m, uint64_t f, size_t idx)
{
    uint64_t L = A376889_L;
    if (m > 1 && f == 2 * m) A376889_record(m);
    if (f >= 2 * m) return;

    /* abundancy upper bound on what remaining primes can add */
    {
        uint64_t B = L / m;
        double R = 1.0;
        uint64_t prod = 1;
        size_t j = idx;
        while (j < A376889_np) {
            uint64_t q = A376889_pr[j];
            if (prod > B / q) break;
            prod *= q;
            R *= (double)q / (double)(q - 1);
            j++;
        }
        if (j >= A376889_np && A376889_np > 0) {
            uint64_t q = A376889_pr[A376889_np - 1];
            R *= (double)q / (double)(q - 1);
        }
        if ((double)f / (double)m * R < 2.0 * (1.0 - 1e-9)) return;
    }

    /* closed form: one more prime q with exponent 1 */
    {
        uint64_t d = 2 * m - f;
        if (f % d == 0) {
            uint64_t q = f / d;
            uint64_t last = idx > 0 ? A376889_pr[idx - 1] : 1;
            if (q > last && q <= L / m && A376889_isprime(q))
                A376889_record(m * q);
        }
    }

    for (size_t j = idx; j < A376889_np; j++) {
        uint64_t q = A376889_pr[j];
        if (q > L / m / q) break;
        uint64_t pe = q;
        uint64_t e = 1;
        uint64_t lim = L / m;
        while (pe <= lim) {
            uint64_t fe = A376889_fpp(q, e);
            A376889_dfs(m * pe, f * fe, j + 1);
            if (pe > lim / q) break;
            pe *= q;
            e++;
        }
    }
}

static void A376889_run(uint64_t L)
{
    uint64_t s = 1;
    while (s * s <= L) s++;
    size_t lim = (size_t)s + 1000;
    char *sv = (char *)malloc(lim + 1);
    memset(sv, 1, lim + 1);
    free(A376889_pr);
    A376889_pr = (uint64_t *)malloc(sizeof(uint64_t) * (lim + 1));
    A376889_np = 0;
    for (size_t i = 2; i <= lim; i++) {
        if (sv[i]) {
            A376889_pr[A376889_np++] = i;
            for (size_t j = i * i; j <= lim; j += i) sv[j] = 0;
        }
    }
    free(sv);
    A376889_L = L;
    A376889_cnt = 0;
    A376889_dfs(1, 1, 0);
    /* sort + unique */
    for (size_t i = 1; i < A376889_cnt; i++) {
        uint64_t v = A376889_res[i];
        size_t j = i;
        while (j > 0 && A376889_res[j - 1] > v) { A376889_res[j] = A376889_res[j - 1]; j--; }
        A376889_res[j] = v;
    }
    size_t w = 0;
    for (size_t i = 0; i < A376889_cnt; i++)
        if (w == 0 || A376889_res[w - 1] != A376889_res[i])
            A376889_res[w++] = A376889_res[i];
    A376889_cnt = w;
    A376889_done_limit = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A376889_done_limit == 0) A376889_run(10000ULL);
    while (A376889_cnt < (size_t)n && A376889_done_limit < 2000000000000000ULL)
        A376889_run(A376889_done_limit * 10ULL);
    if ((size_t)n <= A376889_cnt)
        return (seq_t)A376889_res[n - 1];
    return 0;
}
