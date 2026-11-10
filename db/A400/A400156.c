/*
 * A400156  Intersection of A373487 and A399379.
 * https://oeis.org/A400156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over k: a(n) = n-th k with A276085(k) == 0 (mod
 *            A003415(k)) and k == 0 (mod A007947(A276086(A003415(k)))). A
 *            segmented sieve gives k' = A003415(k) using additions only. A
 *            cheap primorial-digit filter runs next, then a check modulo the
 *            small primes r | k', and finally a full check of A27608...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define A400156_BPLIM 65536u
#define A400156_SEG   65536u
#define A400156_VSEG  (1u << 18)

static uint32_t *A400156_bp = NULL;
static size_t A400156_nbp = 0;
static uint64_t *A400156_found = NULL;
static size_t A400156_cnt = 0, A400156_cap = 0;
static uint64_t A400156_next = 2;

static void A400156_init(void)
{
    if (A400156_bp) return;
    unsigned char *s = (unsigned char *)calloc(A400156_BPLIM + 1, 1);
    A400156_bp = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    for (uint32_t i = 2; i <= A400156_BPLIM; i++) {
        if (!s[i]) {
            A400156_bp[A400156_nbp++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A400156_BPLIM; j += i) s[j] = 1;
        }
    }
    free(s);
    A400156_cap = 64;
    A400156_found = (uint64_t *)malloc(sizeof(uint64_t) * A400156_cap);
    A400156_found[A400156_cnt++] = 1;
}

/* Sum over prime factors q <= lim of k (sorted f[], exps e[]) of
   e_q * (product of primes < q), modulo M. */
static uint64_t A400156_sum(const uint64_t *f, const uint64_t *e, int c,
                            uint64_t M, uint64_t lim)
{
    uint64_t prod = 1 % M;
    unsigned __int128 S = 0;
    int j = 0;
    size_t i;
    for (i = 0; i < A400156_nbp && j < c; i++) {
        uint64_t p = A400156_bp[i];
        if (p > lim) return (uint64_t)(S % M);
        while (j < c && f[j] == p) {
            S = (S + (unsigned __int128)(e[j] % M) * prod) % M;
            j++;
        }
        prod = (uint64_t)(((unsigned __int128)prod * p) % M);
    }
    if (j < c) {
        unsigned char *seg = (unsigned char *)malloc(A400156_VSEG);
        uint64_t lo = (uint64_t)A400156_BPLIM + 1;
        while (j < c && lo <= lim) {
            uint64_t hi = lo + A400156_VSEG;
            for (uint64_t t = 0; t < A400156_VSEG; t++) seg[t] = 0;
            for (i = 0; i < A400156_nbp; i++) {
                uint64_t p = A400156_bp[i];
                if (p * p >= hi) break;
                uint64_t m = ((lo + p - 1) / p) * p;
                if (m < p * p) m = p * p;
                for (; m < hi; m += p) seg[m - lo] = 1;
            }
            for (uint64_t t = 0; t < A400156_VSEG && j < c; t++) {
                if (seg[t]) continue;
                uint64_t r = lo + t;
                if (r > lim) { j = c; break; }
                while (j < c && f[j] == r) {
                    S = (S + (unsigned __int128)(e[j] % M) * prod) % M;
                    j++;
                }
                prod = (uint64_t)(((unsigned __int128)prod * r) % M);
            }
            lo = hi;
        }
        free(seg);
    }
    return (uint64_t)(S % M);
}

static bool A400156_check(uint64_t k, uint64_t D)
{
    if (D <= 1) return true;
    uint64_t f[40], e[40];
    int c = 0;
    uint64_t x = k;
    for (size_t i = 0; i < A400156_nbp; i++) {
        uint64_t p = A400156_bp[i];
        if (p * p > x) break;
        if (x % p == 0) {
            uint64_t ee = 0;
            while (x % p == 0) { x /= p; ee++; }
            f[c] = p; e[c] = ee; c++;
        }
    }
    if (x > 1) { f[c] = x; e[c] = 1; c++; }

    /* quick filter: modulo each small prime r | D only q <= r contribute */
    uint64_t y = D;
    for (size_t i = 0; i < A400156_nbp && y > 1; i++) {
        uint64_t r = A400156_bp[i];
        if (y % r) continue;
        while (y % r == 0) y /= r;
        if (A400156_sum(f, e, c, r, r) != 0) return false;
    }
    /* full verification */
    return A400156_sum(f, e, c, D, f[c - 1]) == 0;
}

static void A400156_search(void)
{
    static uint64_t der[A400156_SEG];
    static uint32_t prd[A400156_SEG];
    static const unsigned sp[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t lo = A400156_next;
    uint64_t hi = lo + A400156_SEG;
    for (uint32_t i = 0; i < A400156_SEG; i++) { der[i] = 0; prd[i] = 1; }
    for (size_t i = 0; i < A400156_nbp; i++) {
        uint64_t p = A400156_bp[i];
        if (p * p >= hi) break;
        for (uint64_t pw = p; pw < hi; pw *= p) {
            uint64_t m = ((lo + pw - 1) / pw) * pw;
            uint64_t step = pw / p;
            uint64_t val = m / p;
            for (; m < hi; m += pw, val += step) {
                der[m - lo] += val;
                prd[m - lo] *= (uint32_t)p;
            }
            if (pw > hi / p) break;
        }
    }
    for (uint32_t i = 0; i < A400156_SEG; i++) {
        uint64_t k = lo + i;
        uint64_t D = der[i];
        if ((uint64_t)prd[i] < k) D += prd[i];
        uint64_t x = D;
        bool ok = true;
        for (int t = 0; x && t < 20; t++) {
            uint64_t d = x % sp[t];
            if (d && (k % sp[t])) { ok = false; break; }
            x /= sp[t];
        }
        if (!ok) continue;
        if (A400156_check(k, D)) {
            if (A400156_cnt == A400156_cap) {
                A400156_cap *= 2;
                A400156_found = (uint64_t *)realloc(A400156_found, sizeof(uint64_t) * A400156_cap);
            }
            A400156_found[A400156_cnt++] = k;
        }
    }
    A400156_next = hi;
}

seq_t a(idx_t n)
{
    A400156_init();
    while (A400156_cnt < (size_t)n && A400156_next < ((uint64_t)1 << 32))
        A400156_search();
    if ((size_t)n > A400156_cnt) return (seq_t)0;
    return (seq_t)A400156_found[(size_t)n - 1];
}
