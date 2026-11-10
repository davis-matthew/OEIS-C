/*
 * A341045  Numbers k such that k divides A318996(k).
 * https://oeis.org/A341045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over k = 1, 2, 3, ...; k is a term iff k | A318996(k) =
 *            Sum_{d|k} (sigma(k) mod d). Uses a segmented sieve to factor k,
 *            and since every d divides k, sigma mod d = (sigma mod k) mod d
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/isqrt_u64.h"

#define A341045_B 32768
#define A341045_MAXF 16

static uint32_t *A341045_primes = NULL;
static size_t A341045_np = 0;
static uint64_t A341045_plim = 0;

static uint64_t *A341045_terms = NULL;
static size_t A341045_cnt = 0, A341045_cap = 0;
static uint64_t A341045_scan = 1;

static uint32_t A341045_fp[A341045_B * A341045_MAXF];
static uint8_t A341045_fe[A341045_B * A341045_MAXF];
static uint8_t A341045_nf[A341045_B];
static uint64_t A341045_prod[A341045_B];

static void A341045_ensure_primes(uint64_t hi)
{
    uint64_t r = isqrt_u64(hi);
    if (r <= A341045_plim)
        return;
    uint64_t lim = r * 2 + 64;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    free(A341045_primes);
    A341045_primes = NULL;
    A341045_np = 0;
    size_t cap = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!comp[i]) {
            if (A341045_np == cap) {
                cap = cap ? cap * 2 : 1024;
                A341045_primes = (uint32_t *)realloc(A341045_primes, cap * sizeof(uint32_t));
            }
            A341045_primes[A341045_np++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i)
                comp[j] = 1;
        }
    }
    free(comp);
    A341045_plim = lim;
}

static void A341045_push(uint64_t k)
{
    if (A341045_cnt == A341045_cap) {
        A341045_cap = A341045_cap ? A341045_cap * 2 : 64;
        A341045_terms = (uint64_t *)realloc(A341045_terms, A341045_cap * sizeof(uint64_t));
    }
    A341045_terms[A341045_cnt++] = k;
}

static void A341045_test(uint64_t k, size_t idx)
{
    static uint64_t divs[8192];
    size_t nd = 1;
    divs[0] = 1;
    uint64_t sig = 1;
    const uint32_t *fp = A341045_fp + idx * A341045_MAXF;
    const uint8_t *fe = A341045_fe + idx * A341045_MAXF;
    for (int t = 0; t < A341045_nf[idx]; ++t) {
        uint64_t p = fp[t];
        size_t prev = nd;
        uint64_t pw = 1, ps = 1;
        for (int j = 0; j < fe[t]; ++j) {
            pw *= p;
            ps += pw;
            for (size_t i = 0; i < prev; ++i)
                divs[nd + i] = divs[nd - prev + i] * p;
            nd += prev;
        }
        sig *= ps;
    }
    /* divs[nd-1-i] == k / divs[i] by construction (mixed-radix layout) */
    uint64_t s = sig % k;
    uint64_t inv = UINT64_MAX / k;
    uint64_t sum = 0;
    for (size_t i = 0; i < nd; ++i) {
        uint64_t d = divs[i];
        if (d > s) {
            sum += s;
            continue;
        }
        uint64_t e = divs[nd - 1 - i];
        uint64_t x = s * e; /* floor(s/d) = floor(s*e/k) */
        uint64_t q = (uint64_t)(((unsigned __int128)x * inv) >> 64);
        uint64_t r = x - q * k;
        while (r >= k) {
            r -= k;
            ++q;
        }
        sum += s - d * q;
    }
    if (sum % k == 0)
        A341045_push(k);
}

static void A341045_block(uint64_t L)
{
    uint64_t hi = L + A341045_B - 1;
    A341045_ensure_primes(hi);
    memset(A341045_nf, 0, sizeof(A341045_nf));
    for (size_t i = 0; i < A341045_B; ++i)
        A341045_prod[i] = 1;
    for (size_t t = 0; t < A341045_np; ++t) {
        uint64_t p = A341045_primes[t];
        if (p * p > hi)
            break;
        uint64_t pw = p;
        for (;;) {
            uint64_t start = ((L + pw - 1) / pw) * pw;
            for (uint64_t j = start; j <= hi; j += pw) {
                size_t idx = (size_t)(j - L);
                if (pw == p) {
                    int c = A341045_nf[idx]++;
                    A341045_fp[idx * A341045_MAXF + c] = (uint32_t)p;
                    A341045_fe[idx * A341045_MAXF + c] = 1;
                } else {
                    A341045_fe[idx * A341045_MAXF + A341045_nf[idx] - 1]++;
                }
                A341045_prod[idx] *= p;
            }
            if (pw > hi / p)
                break;
            pw *= p;
        }
    }
    for (size_t idx = 0; idx < A341045_B; ++idx) {
        uint64_t k = L + idx;
        if (A341045_prod[idx] < k) {
            int c = A341045_nf[idx]++;
            A341045_fp[idx * A341045_MAXF + c] = (uint32_t)(k / A341045_prod[idx]);
            A341045_fe[idx * A341045_MAXF + c] = 1;
        }
        A341045_test(k, idx);
    }
}

seq_t a(idx_t n)
{
    while (A341045_cnt < (size_t)n) {
        A341045_block(A341045_scan);
        A341045_scan += A341045_B;
    }
    return (seq_t)A341045_terms[n - 1];
}
