/*
 * A356021  Positive numbers k such that, for any consecutive prime numbers p,
 *          q <= A006530(n), the p-adic and q-adic valuations of n are
 *          different.
 * https://oeis.org/A356021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that v_p(k) != v_q(k) for all
 *            consecutive primes p<q<=A006530(k); found by incremental search
 *            with trial division by a sieved prime list
 * Range:     u8 n <= 46, u16 n <= 632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9668 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *pl_pr = NULL;
static size_t pl_cnt = 0;
static uint64_t pl_lim = 0;

static void pl_ensure(uint64_t need)
{
    if (need <= pl_lim) return;
    uint64_t lim = pl_lim * 2;
    if (lim < need) lim = need;
    if (lim < 1024) lim = 1024;
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!s) abort();
    free(pl_pr);
    pl_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    if (!pl_pr) abort();
    pl_cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            pl_pr[pl_cnt++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
    pl_lim = lim;
}

static bool A356021_test(uint64_t k)
{
    if (k == 1) return true;
    pl_ensure(k + 2);
    uint64_t m = k;
    int prev = -1;
    size_t i = 0;
    while (i < pl_cnt) {
        uint64_t p = pl_pr[i];
        if (p * p > m) break;
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        if (i > 0 && e == prev) return false;
        prev = e;
        i++;
    }
    if (m > 1) {
        /* m is prime; find its index by binary search */
        size_t lo = 0, hi = pl_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (pl_pr[mid] < m) lo = mid + 1; else hi = mid;
        }
        size_t c = lo - i; /* primes in [p_i, m) */
        if (c >= 2) return false;
        if (c == 1) { if (prev == 0) return false; }
        else { if (prev == 1) return false; }
    }
    return true;
}

static uint64_t *A356021_terms = NULL;
static size_t A356021_n = 0, A356021_cap = 0;
static uint64_t A356021_k = 0;

seq_t a(idx_t n)
{
    while (A356021_n < (size_t)n) {
        A356021_k++;
        if (A356021_test(A356021_k)) {
            if (A356021_n == A356021_cap) {
                A356021_cap = A356021_cap ? A356021_cap * 2 : 1024;
                A356021_terms = (uint64_t *)realloc(A356021_terms, A356021_cap * sizeof(uint64_t));
                if (!A356021_terms) abort();
            }
            A356021_terms[A356021_n++] = A356021_k;
        }
    }
    return (seq_t)A356021_terms[(size_t)n - 1];
}
