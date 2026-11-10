/*
 * A341061  Numbers k such that A340179(k) is prime.
 * https://oeis.org/A341061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A340179(k) is prime, where A340179(k) =
 *            Sum_{x<k, gcd(x,k)=1} ((k*phi(k)/2) mod x); computed by
 *            incremental search
 * Range:     u8 n <= 20, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 77 terms through n = 2816 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t mr_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}
static uint64_t mr_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1; b %= m;
    while (e) { if (e & 1) r = mr_mulmod(r, b, m); b = mr_mulmod(b, b, m); e >>= 1; }
    return r;
}
static int mr_prime(uint64_t n) {
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return 0;
    for (int i = 0; i < 12; i++) {
        if (n == bs[i]) return 1;
        if (n % bs[i] == 0) return 0;
    }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = mr_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (int r = 1; r < s; r++) {
            x = mr_mulmod(x, x, n);
            if (x == n - 1) { ok = 1; break; }
        }
        if (!ok) return 0;
    }
    return 1;
}

static unsigned char *A341061_mark = NULL;
static uint64_t A341061_markcap = 0;

static uint64_t A341061_val(uint64_t k) {
    if (k + 1 > A341061_markcap) {
        free(A341061_mark);
        A341061_markcap = (k + 1) * 2;
        A341061_mark = (unsigned char *)malloc(A341061_markcap);
    }
    memset(A341061_mark, 0, k + 1);
    uint64_t m = k, phi = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi = phi / p * (p - 1);
            for (uint64_t j = p; j < k; j += p) A341061_mark[j] = 1;
        }
    }
    if (m > 1) {
        phi = phi / m * (m - 1);
        for (uint64_t j = m; j < k; j += m) A341061_mark[j] = 1;
    }
    uint64_t c = k * phi / 2;
    uint64_t sum = 0;
    if (c < 0xFFFFFFFFULL) {
        uint32_t c32 = (uint32_t)c;
        for (uint32_t x = 1; x < (uint32_t)k; x++)
            if (!A341061_mark[x]) sum += c32 % x;
    } else {
        for (uint64_t x = 1; x < k; x++)
            if (!A341061_mark[x]) sum += c % x;
    }
    return sum;
}

static uint64_t *A341061_terms = NULL;
static uint64_t A341061_cnt = 0, A341061_cap = 0, A341061_k = 0;

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (A341061_cnt < (uint64_t)n) {
        A341061_k++;
        uint64_t k = A341061_k;
        uint64_t v = (k == 1) ? 0 : A341061_val(k);
        if (k == 1) {
            /* C(1) = {} per definition => sum 0; but OEIS a(1)=5 so k=1 is not a term */
        }
        if (v >= 2 && mr_prime(v)) {
            if (A341061_cnt == A341061_cap) {
                A341061_cap = A341061_cap ? A341061_cap * 2 : 1024;
                A341061_terms = (uint64_t *)realloc(A341061_terms, A341061_cap * sizeof(uint64_t));
            }
            A341061_terms[A341061_cnt++] = k;
        }
    }
    return (seq_t)A341061_terms[n - 1];
}
