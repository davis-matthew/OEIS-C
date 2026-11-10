/*
 * A389296  Smallest k for which A386476(k) = n.
 * https://oeis.org/A389296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that A386476(k) = n, where A386476(k) = #{d
 *            | k : gcd(d^2-1, k) = 1}; found by an incremental search over k
 *            with a smallest-prime-factor sieve
 * Range:     u8 n <= 9, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A389296_spf = 0;
static uint64_t A389296_L = 0;
static uint64_t A389296_next = 1;
static uint64_t A389296_first[256];

static void A389296_grow(uint64_t need) {
    uint64_t L = A389296_L ? A389296_L : (1u << 20);
    while (L < need) L *= 2;
    if (L == A389296_L) return;
    if (A389296_spf) free(A389296_spf);
    A389296_spf = (uint32_t *)calloc((size_t)L + 1, sizeof(uint32_t));
    A389296_L = L;
    for (uint64_t i = 2; i <= L; i++) {
        if (A389296_spf[i] == 0) {
            A389296_spf[i] = (uint32_t)i;
            if (i * i <= L)
                for (uint64_t j = i * i; j <= L; j += i)
                    if (A389296_spf[j] == 0) A389296_spf[j] = (uint32_t)i;
        }
    }
}

static unsigned A389296_f(uint64_t k) {
    uint32_t pr[16];
    int ex[16];
    int np = 0;
    uint64_t m = k;
    while (m > 1) {
        uint32_t p = A389296_spf[m];
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        pr[np] = p; ex[np] = e; np++;
    }
    static uint64_t divs[8192];
    size_t nd = 1;
    divs[0] = 1;
    for (int i = 0; i < np; i++) {
        size_t cur = nd;
        uint64_t pw = 1;
        for (int e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
        }
    }
    unsigned cnt = 0;
    for (size_t j = 0; j < nd; j++) {
        uint64_t d = divs[j];
        int ok = 1;
        for (int i = 0; i < np; i++) {
            uint64_t r = d % pr[i];
            if (r == 1 || r == (uint64_t)pr[i] - 1) { ok = 0; break; }
        }
        if (ok) cnt++;
    }
    return cnt;
}

seq_t a(idx_t n) {
    if (n >= 256) return (seq_t)0;
    while (A389296_first[n] == 0) {
        uint64_t k = A389296_next++;
        if (k > A389296_L) A389296_grow(k);
        unsigned f = A389296_f(k);
        if (f < 256 && A389296_first[f] == 0) A389296_first[f] = k;
    }
    return (seq_t)A389296_first[n];
}
