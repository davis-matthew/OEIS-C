/*
 * A270474  Integers k such that A256832(k) is not divisible by k*(k+1)/2.
 * https://oeis.org/A270474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k>=1 with A256832(k) = Product_{j=1..k}
 *            Pell(j) not divisible by k*(k+1)/2. Search by checking
 *            prime-power valuations of T=k(k+1)/2 against v_p(prod Pell(j)),
 *            using rank of apparition
 * Range:     u8 n <= 22, u16 n <= 2444, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8865 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

typedef unsigned __int128 u128_pp;

static uint32_t *pp_small = 0;
static size_t pp_small_cnt = 0;
static uint32_t *pp_z = 0;
static size_t pp_z_size = 0;

static void pp_init(void) {
    if (pp_small) return;
    const uint32_t L = 65536;
    unsigned char *s = (unsigned char *)calloc(L + 1, 1);
    pp_small = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    for (uint32_t i = 2; i <= L; i++) {
        if (!s[i]) {
            pp_small[pp_small_cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
}

static uint64_t pp_mm(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((u128_pp)a * b % m);
}

/* Pell(n) mod M */
static uint64_t pp_pell(uint64_t n, uint64_t M) {
    uint64_t a = 0, b = 1 % M;
    int top = 63;
    while (top >= 0 && !((n >> top) & 1)) top--;
    for (int i = top; i >= 0; i--) {
        uint64_t d = (2 * b + 2 * M - 2 * a % M) % M;
        uint64_t na = pp_mm(a, d, M);
        uint64_t nb = (pp_mm(a, a, M) + pp_mm(b, b, M)) % M;
        a = na; b = nb;
        if ((n >> i) & 1) {
            uint64_t t = (2 * b + a) % M;
            a = b; b = t;
        }
    }
    return a;
}

static uint64_t pp_rank(uint64_t p) {
    uint64_t N = ((p & 7) == 1 || (p & 7) == 7) ? p - 1 : p + 1;
    uint64_t z = N;
    uint64_t x = N;
    for (size_t i = 0; i < pp_small_cnt && (uint64_t)pp_small[i] * pp_small[i] <= x; i++) {
        uint64_t q = pp_small[i];
        if (x % q == 0) {
            while (x % q == 0) x /= q;
            while (z % q == 0 && pp_pell(z / q, p) == 0) z /= q;
        }
    }
    if (x > 1) {
        uint64_t q = x;
        if (z % q == 0 && pp_pell(z / q, p) == 0) z /= q;
    }
    return z;
}

static uint64_t pp_getz(uint64_t p) {
    if (p >= pp_z_size) {
        size_t ns = pp_z_size ? pp_z_size : 1024;
        while (ns <= p) ns *= 2;
        pp_z = (uint32_t *)realloc(pp_z, ns * sizeof(uint32_t));
        memset(pp_z + pp_z_size, 0, (ns - pp_z_size) * sizeof(uint32_t));
        pp_z_size = ns;
    }
    if (!pp_z[p]) pp_z[p] = (uint32_t)pp_rank(p);
    return pp_z[p];
}

/* check prime power p^e divides the product up to k */
static int pp_prime_ok(uint64_t k, uint64_t p, int e) {
    if (e <= 0) return 1;
    if (p == 2) {
        uint64_t v = 0, t = k;
        while (t) { t >>= 1; v += t; }
        return v >= (uint64_t)e;
    }
    uint64_t z = pp_getz(p);
    uint64_t c = k / z;
    uint64_t vf = 0, t = c;
    while (t) { t /= p; vf += t; }
    uint64_t vp = c + vf;
    if (vp >= (uint64_t)e) return 1;
    if (c == 0) return 0;
    uint64_t M = 1;
    for (int i = 0; i < e; i++) M *= p;
    uint64_t r = pp_pell(z, M);
    uint64_t a = 0;
    if (r == 0) a = e;
    else while (r % p == 0) { r /= p; a++; }
    return c * a + vf >= (uint64_t)e;
}

static int pp_factor_check(uint64_t k, uint64_t x, int adj2, uint64_t kk) {
    for (size_t i = 0; i < pp_small_cnt && (uint64_t)pp_small[i] * pp_small[i] <= x; i++) {
        uint64_t q = pp_small[i];
        if (x % q == 0) {
            int e = 0;
            while (x % q == 0) { x /= q; e++; }
            if (q == 2) e -= adj2;
            if (!pp_prime_ok(kk, q, e)) return 0;
        }
    }
    if (x > 1) {
        int e = 1;
        if (x == 2) e -= adj2;
        if (!pp_prime_ok(kk, x, e)) return 0;
    }
    (void)k;
    return 1;
}

/* returns 1 if T divides product (not a term) */
static int pp_divisible(uint64_t k) {
    if (k <= 1) return 1;
    if (!pp_factor_check(k, k, 1, k)) return 0;
    if (!pp_factor_check(k, k + 1, 0, k)) return 0;
    return 1;
}

static uint64_t *pp_terms = 0;
static size_t pp_tcnt = 0, pp_tcap = 0;
static uint64_t pp_last = 0;

seq_t a(idx_t n) {
    pp_init();
    while (pp_tcnt < (size_t)n) {
        pp_last++;
        if (!pp_divisible(pp_last)) {
            if (pp_tcnt == pp_tcap) {
                pp_tcap = pp_tcap ? pp_tcap * 2 : 1024;
                pp_terms = (uint64_t *)realloc(pp_terms, pp_tcap * sizeof(uint64_t));
            }
            pp_terms[pp_tcnt++] = pp_last;
        }
    }
    return (seq_t)pp_terms[n - 1];
}
