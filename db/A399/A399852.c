/*
 * A399852  Numbers k >= 2 for which 2k is missing in A399704.
 * https://oeis.org/A399852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Let t = 2k. If t+1 is composite, then A399704(t) = t, so
 *            t is never missing. A399704(m) = m exactly when m+1 is
 *            composite. So a(n) runs through the k >= 2 such that 2k+1 is
 *            prime and 2k != A399704(m) for every m < 2k with m+1 prime.
 *            A399704(m) is the smallest a >= m with m(m-1)/2 + a | (m...
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9862 (2026-11-10)
 * Helpers:   gcd_u64, is_prime_u64, isqrt_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/gcd_u64.h"
#include "../utils/isqrt_u64.h"

static uint32_t *A399852_sp = 0;
static size_t A399852_nsp = 0;

static void A399852_init_primes(void)
{
    if (A399852_sp) return;
    const uint32_t L = 70000;
    char *comp = (char *)calloc(L + 1, 1);
    size_t cnt = 0;
    for (uint32_t i = 2; i <= L; i++) {
        if (!comp[i]) {
            cnt++;
            for (uint64_t j = (uint64_t)i * i; j <= L; j += i) comp[j] = 1;
        }
    }
    A399852_sp = (uint32_t *)malloc(sizeof(uint32_t) * cnt);
    size_t c = 0;
    for (uint32_t i = 2; i <= L; i++)
        if (!comp[i]) A399852_sp[c++] = i;
    A399852_nsp = c;
    free(comp);
}

static uint64_t A399852_rho(uint64_t n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            d = gcd_u64(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

/* factor r into primes (pr, ex), returns count */
static int A399852_factor(uint64_t r, uint64_t *pr, int *ex)
{
    int cnt = 0;
    for (size_t i = 0; i < A399852_nsp; i++) {
        uint64_t p = A399852_sp[i];
        if (p * p * p > r) break;
        if (r % p == 0) {
            int e = 0;
            while (r % p == 0) { r /= p; e++; }
            pr[cnt] = p; ex[cnt] = e; cnt++;
        }
    }
    if (r > 1) {
        if (is_prime_u64(r)) { pr[cnt] = r; ex[cnt] = 1; cnt++; }
        else {
            uint64_t q = isqrt_u64(r);
            if (q * q == r) { pr[cnt] = q; ex[cnt] = 2; cnt++; }
            else {
                uint64_t f = A399852_rho(r);
                uint64_t g = r / f;
                if (f == g) { pr[cnt] = f; ex[cnt] = 2; cnt++; }
                else {
                    pr[cnt] = f; ex[cnt] = 1; cnt++;
                    pr[cnt] = g; ex[cnt] = 1; cnt++;
                }
            }
        }
    }
    return cnt;
}

/* does (m-1)m/2 + a divide (m-1)! * a ? */
static bool A399852_cond(uint64_t m, uint64_t a)
{
    uint64_t S = m * (m - 1) / 2 + a;
    uint64_t pr[64];
    int ex[64];
    int cnt = A399852_factor(S, pr, ex);
    uint64_t N = m - 1;
    for (int i = 0; i < cnt; i++) {
        uint64_t q = pr[i];
        int e = ex[i];
        int v = 0;
        uint64_t aa = a;
        while (v < e && aa % q == 0) { aa /= q; v++; }
        if (v >= e) continue;
        if (q <= N) {
            uint64_t qq = q;
            while (qq <= N && v < e) {
                v += (int)(N / qq);
                if (qq > N / q) break;
                qq *= q;
            }
        }
        if (v < e) return false;
    }
    return true;
}

static unsigned char *A399852_mark = 0;
static size_t A399852_markcap = 0;
static uint64_t A399852_doneM = 2;

static void A399852_set_mark(uint64_t t)
{
    if (t >= A399852_markcap) {
        size_t nc = A399852_markcap ? A399852_markcap : 1024;
        while (nc <= t) nc *= 2;
        A399852_mark = (unsigned char *)realloc(A399852_mark, nc);
        memset(A399852_mark + A399852_markcap, 0, nc - A399852_markcap);
        A399852_markcap = nc;
    }
    A399852_mark[t] = 1;
}

static void A399852_process(uint64_t m)
{
    if (m < 3) return;
    if (!is_prime_u64(m + 1)) return; /* A399704(m) = m, m+1 composite: irrelevant */
    uint64_t a = m + 1;
    while (!A399852_cond(m, a)) a++;
    A399852_set_mark(a);
}

static uint64_t *A399852_terms = 0;
static size_t A399852_cnt = 0, A399852_cap = 0;
static uint64_t A399852_nextk = 2;

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A399852_init_primes();
    while (A399852_cnt < (size_t)n) {
        uint64_t k = A399852_nextk++;
        uint64_t t = 2 * k;
        while (A399852_doneM < t - 1) A399852_process(++A399852_doneM);
        if (is_prime_u64(t + 1) && !(t < A399852_markcap && A399852_mark[t])) {
            if (A399852_cnt == A399852_cap) {
                A399852_cap = A399852_cap ? A399852_cap * 2 : 64;
                A399852_terms = (uint64_t *)realloc(A399852_terms, A399852_cap * sizeof(uint64_t));
            }
            A399852_terms[A399852_cnt++] = k;
        }
    }
    return (seq_t)A399852_terms[n - 1];
}
