/*
 * A342174  Primes in A342173.
 * https://oeis.org/A342174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = k-th prime among the values A342173(n) = Sum_{j=1..n-1}
 *            floor(prime(n)/prime(j)), n = 1, 2, ...; found by scanning n in
 *            increasing order, with the terms memoized
 * Range:     u8 n <= 9, u16 n <= 281, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 856 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *p342174_pr = NULL;
static size_t p342174_cnt = 0;
static uint64_t p342174_lim = 0;

static void p342174_ensure(size_t n)
{
    while (p342174_cnt < n) {
        uint64_t lim = p342174_lim ? p342174_lim * 2 : 1024;
        char *s = (char *)calloc((size_t)lim + 1, 1);
        if (!s) abort();
        size_t c = 0;
        for (uint64_t i = 2; i <= lim; i++) {
            if (!s[i]) {
                c++;
                for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
            }
        }
        free(p342174_pr);
        p342174_pr = (uint32_t *)malloc((c ? c : 1) * sizeof(uint32_t));
        if (!p342174_pr) abort();
        size_t k = 0;
        for (uint64_t i = 2; i <= lim; i++)
            if (!s[i]) p342174_pr[k++] = (uint32_t)i;
        p342174_cnt = c;
        p342174_lim = lim;
        free(s);
    }
}

static bool p342174_isprime(uint64_t v)
{
    if (v < 2) return false;
    if (v % 2 == 0) return v == 2;
    for (uint64_t d = 3; d * d <= v; d += 2)
        if (v % d == 0) return false;
    return true;
}

static uint64_t *p342174_terms = NULL;
static size_t p342174_nterms = 0, p342174_tcap = 0;
static size_t p342174_next = 1;

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    while (p342174_nterms < k) {
        size_t m = p342174_next++;
        p342174_ensure(m);
        uint64_t pm = p342174_pr[m - 1];
        uint64_t sum = 0;
        for (size_t j = 0; j + 1 < m; j++)
            sum += pm / p342174_pr[j];
        if (p342174_isprime(sum)) {
            if (p342174_nterms == p342174_tcap) {
                p342174_tcap = p342174_tcap ? p342174_tcap * 2 : 64;
                p342174_terms = (uint64_t *)realloc(p342174_terms, p342174_tcap * sizeof(uint64_t));
                if (!p342174_terms) abort();
            }
            p342174_terms[p342174_nterms++] = sum;
        }
    }
    return (seq_t)p342174_terms[k - 1];
}
