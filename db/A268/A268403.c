/*
 * A268403  Partial sums of A080715.
 * https://oeis.org/A268403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A080715(k); A080715 terms found by
 *            sieve-assisted search
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 3029, u64 n <= 39627,
 *            u128 n <= 39627, big any n
 * Validated: standard, pass, 61 terms through n = 9862 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* the terms of A080715 found so far, in an array that grows (this was a fixed
   array of 20000, and a(n) for larger n silently returned a(20000)) */
static uint64_t *A268403_terms = NULL;
static size_t A268403_cnt = 0, A268403_cap = 0;
static uint64_t A268403_scanned = 0;   /* all k <= scanned have been tested */

static uint8_t *A268403_sv = NULL;     /* odd-only composite bitmap */
static uint64_t A268403_svM = 0;       /* sieve covers numbers <= svM */
static uint32_t *A268403_pr = NULL;    /* odd primes up to sqrt */
static size_t A268403_prn = 0;

static bool A268403_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (!(x & 1ULL)) return false;
    uint64_t i = x >> 1;
    return !((A268403_sv[i >> 3] >> (i & 7)) & 1);
}

static void A268403_build(uint64_t M)
{
    free(A268403_sv);
    free(A268403_pr);
    size_t bytes = (size_t)((M >> 1) / 8 + 2);
    A268403_sv = (uint8_t *)calloc(bytes, 1);
    if (!A268403_sv) exit(1);
    A268403_svM = M;
    A268403_sv[0] |= 1; /* 1 is not prime */
    for (uint64_t i = 3; i * i <= M; i += 2) {
        uint64_t bi = i >> 1;
        if ((A268403_sv[bi >> 3] >> (bi & 7)) & 1) continue;
        for (uint64_t j = i * i; j <= M; j += 2 * i) {
            uint64_t bj = j >> 1;
            A268403_sv[bj >> 3] |= (uint8_t)(1u << (bj & 7));
        }
    }
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= M) ++r;
    A268403_pr = (uint32_t *)malloc((size_t)(r / 2 + 2) * sizeof(uint32_t));
    if (!A268403_pr) exit(1);
    A268403_prn = 0;
    for (uint64_t p = 3; p <= r + 1 && p <= M; p += 2)
        if (A268403_isp(p)) A268403_pr[A268403_prn++] = (uint32_t)p;
}

static bool A268403_good(uint64_t k)
{
    if (k == 1 || k == 2) return true;
    if ((k & 3ULL) != 2) return false;
    if (!A268403_isp(k + 1)) return false;
    if (!A268403_isp(k / 2 + 2)) return false;
    uint64_t fac[40];
    int nf = 0;
    fac[nf++] = 2;
    uint64_t m = k / 2;
    for (size_t i = 0; i < A268403_prn; ++i) {
        uint64_t p = A268403_pr[i];
        if (p * p > m) break;
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return false;
            if (!A268403_isp(p + k / p)) return false;
            if (!A268403_isp(2 * p + k / (2 * p))) return false;
            fac[nf++] = p;
        }
    }
    if (m > 1) fac[nf++] = m;
    uint32_t lim = 1u << nf;
    for (uint32_t mask = 0; mask < lim; ++mask) {
        uint64_t d = 1;
        for (int j = 0; j < nf; ++j)
            if (mask & (1u << j)) d *= fac[j];
        if (d * d > k) continue;
        if (!A268403_isp(d + k / d)) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A268403_cnt < (size_t)n) {
        uint64_t L = A268403_svM ? (A268403_svM - 1) * 2 : (1ULL << 16);
        A268403_build(L + 1);
        for (uint64_t k = A268403_scanned + 1; k <= L; ++k) {
            if (A268403_good(k)) {
                if (A268403_cnt == A268403_cap) {
                    A268403_cap = A268403_cap ? 2 * A268403_cap : 1024;
                    uint64_t *t = (uint64_t *)realloc(A268403_terms, A268403_cap * sizeof *t);
                    if (!t) exit(1);
                    A268403_terms = t;
                }
                A268403_terms[A268403_cnt++] = k;
            }
            A268403_scanned = k;
        }
    }
    seq_t s = 0;
    for (size_t i = 0; i < (size_t)n; ++i)
        s += (seq_t)A268403_terms[i];
    return s;
}
