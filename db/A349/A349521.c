/*
 * A349521  Numbers k such that A348172(k) = 1.
 * https://oeis.org/A349521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A348172(k) = 1, i.e. tau(m)/m = tau(k)/k
 *            only for m = k (search; m = q*t with t <= 4q/p^2, p/q = tau(k)/k
 *            reduced)
 * Range:     u8 n <= 39, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A349521_spf = 0;
static uint64_t A349521_spf_n = 0;

static void A349521_ensure(uint64_t need)
{
    if (need <= A349521_spf_n) return;
    uint64_t sz = A349521_spf_n ? A349521_spf_n : 1024;
    while (sz < need) sz *= 2;
    if (A349521_spf) free(A349521_spf);
    A349521_spf = (uint32_t *)malloc((sz + 1) * sizeof(uint32_t));
    for (uint64_t i = 0; i <= sz; i++) A349521_spf[i] = 0;
    for (uint64_t i = 2; i <= sz; i++) {
        if (A349521_spf[i] == 0) {
            for (uint64_t j = i; j <= sz; j += i)
                if (A349521_spf[j] == 0) A349521_spf[j] = (uint32_t)i;
        }
    }
    A349521_spf_n = sz;
}

static uint64_t A349521_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* returns true if k has no other m with tau(m)/m = tau(k)/k */
static bool A349521_unique(uint64_t k)
{
    uint64_t qp[24], qe[24];
    int qc = 0;
    uint64_t x = k, tau = 1;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            tau *= e + 1;
        }
    }
    if (x > 1) tau *= 2;
    uint64_t g = A349521_gcd(tau, k);
    uint64_t p = tau / g, q = k / g;
    /* factor q */
    x = q;
    for (uint64_t d = 2; d * d <= x; d++) {
        if (x % d == 0) {
            uint64_t e = 0;
            while (x % d == 0) { x /= d; e++; }
            qp[qc] = d; qe[qc] = e; qc++;
        }
    }
    if (x > 1) { qp[qc] = x; qe[qc] = 1; qc++; }
    uint64_t tmax = 4 * q / (p * p) + 1;
    A349521_ensure(tmax);
    int count = 0;
    for (uint64_t t = 1; t <= tmax; t++) {
        uint64_t used[24] = {0};
        uint64_t td = 1;
        uint64_t y = t;
        while (y > 1) {
            uint64_t r = A349521_spf[y];
            uint64_t e = 0;
            while (y % r == 0) { y /= r; e++; }
            for (int i = 0; i < qc; i++)
                if (qp[i] == r) { e += qe[i]; used[i] = 1; break; }
            td *= e + 1;
        }
        for (int i = 0; i < qc; i++)
            if (!used[i]) td *= qe[i] + 1;
        if (td == p * t) {
            if (++count > 1) return false;
        }
    }
    return count == 1;
}

static uint64_t *A349521_terms = 0;
static uint64_t A349521_cnt = 0, A349521_cap = 0, A349521_next = 1;

seq_t a(idx_t n)
{
    while (A349521_cnt < n) {
        uint64_t k = A349521_next++;
        if (A349521_unique(k)) {
            if (A349521_cnt == A349521_cap) {
                A349521_cap = A349521_cap ? A349521_cap * 2 : 256;
                A349521_terms = (uint64_t *)realloc(A349521_terms, A349521_cap * sizeof(uint64_t));
            }
            A349521_terms[A349521_cnt++] = k;
        }
    }
    return (seq_t)A349521_terms[n - 1];
}
