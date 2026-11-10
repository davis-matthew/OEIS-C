/*
 * A309197  List of numbers k such that A111273(k) reaches the smallest
 *          missing number.
 * https://oeis.org/A309197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A111273(k) equals the smallest number
 *            not yet among A111273(1..k-1); computed by simulating A111273
 *            (search with a hash set of seen values)
 * Range:     u8 n <= 32, u16 n <= 3019, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8514 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A309197_hs = NULL;
static size_t A309197_cap = 0, A309197_cnt = 0;
static uint64_t *A309197_terms = NULL;
static size_t A309197_nt = 0, A309197_tcap = 0;
static uint64_t A309197_k = 0;
static uint64_t A309197_m = 1;
static uint64_t *A309197_divs = NULL;
static size_t A309197_dcap = 0;

static bool A309197_has(uint64_t v)
{
    if (!A309197_cap) return false;
    size_t mask = A309197_cap - 1;
    size_t i = (size_t)((v * 0x9E3779B97F4A7C15ULL) >> 20) & mask;
    while (A309197_hs[i]) {
        if (A309197_hs[i] == v) return true;
        i = (i + 1) & mask;
    }
    return false;
}

static void A309197_ins_raw(uint64_t v)
{
    size_t mask = A309197_cap - 1;
    size_t i = (size_t)((v * 0x9E3779B97F4A7C15ULL) >> 20) & mask;
    while (A309197_hs[i]) i = (i + 1) & mask;
    A309197_hs[i] = v;
}

static void A309197_ins(uint64_t v)
{
    if ((A309197_cnt + 1) * 2 > A309197_cap) {
        size_t nc = A309197_cap ? A309197_cap * 2 : 1024;
        uint64_t *old = A309197_hs;
        size_t oc = A309197_cap;
        A309197_hs = (uint64_t *)calloc(nc, sizeof(uint64_t));
        A309197_cap = nc;
        for (size_t i = 0; i < oc; i++)
            if (old[i]) A309197_ins_raw(old[i]);
        free(old);
    }
    A309197_ins_raw(v);
    A309197_cnt++;
}

/* extend divisor list by prime powers of x */
static size_t A309197_factor_expand(uint64_t x, size_t len)
{
    for (uint64_t p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; e++; }
            size_t base = len;
            uint64_t pw = 1;
            for (int j = 1; j <= e; j++) {
                pw *= p;
                for (size_t t = 0; t < base; t++) {
                    if (len >= A309197_dcap) {
                        A309197_dcap *= 2;
                        A309197_divs = (uint64_t *)realloc(A309197_divs, A309197_dcap * sizeof(uint64_t));
                    }
                    A309197_divs[len++] = A309197_divs[t] * pw;
                }
            }
        }
    }
    if (x > 1) {
        size_t base = len;
        for (size_t t = 0; t < base; t++) {
            if (len >= A309197_dcap) {
                A309197_dcap *= 2;
                A309197_divs = (uint64_t *)realloc(A309197_divs, A309197_dcap * sizeof(uint64_t));
            }
            A309197_divs[len++] = A309197_divs[t] * x;
        }
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A309197_divs) {
        A309197_dcap = 4096;
        A309197_divs = (uint64_t *)malloc(A309197_dcap * sizeof(uint64_t));
    }
    while (A309197_nt < (size_t)n) {
        uint64_t k = ++A309197_k;
        uint64_t x = k, y = k + 1;
        if (x % 2 == 0) x /= 2; else y /= 2;
        A309197_divs[0] = 1;
        size_t len = A309197_factor_expand(x, 1);
        len = A309197_factor_expand(y, len);
        uint64_t best = 0;
        for (size_t i = 0; i < len; i++) {
            uint64_t d = A309197_divs[i];
            if ((best == 0 || d < best) && !A309197_has(d)) best = d;
        }
        A309197_ins(best);
        if (best == A309197_m) {
            if (A309197_nt >= A309197_tcap) {
                A309197_tcap = A309197_tcap ? A309197_tcap * 2 : 256;
                A309197_terms = (uint64_t *)realloc(A309197_terms, A309197_tcap * sizeof(uint64_t));
            }
            A309197_terms[A309197_nt++] = k;
            while (A309197_has(A309197_m)) A309197_m++;
        }
    }
    return (seq_t)A309197_terms[(size_t)n - 1];
}
