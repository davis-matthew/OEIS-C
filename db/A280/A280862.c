/*
 * A280862  Numbers n such that A258409(n) * A002322(n) = A000010(n), where
 *          A258409(1) = 1.
 * https://oeis.org/A280862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for n with A258409(n)*A002322(n) = A000010(n),
 *            A258409(1)=1; terms found by sieve-based scan and cached
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9960 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *a280862_spf = 0;
static uint64_t a280862_lim = 0;
static uint64_t *a280862_terms = 0;
static uint64_t a280862_cnt = 0, a280862_cap = 0;
static uint64_t a280862_next = 1;

static uint64_t a280862_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void a280862_sieve(uint64_t lim) {
    free(a280862_spf);
    a280862_spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    a280862_lim = lim;
    for (uint64_t i = 2; i <= lim; i++) {
        if (a280862_spf[i] == 0) {
            for (uint64_t j = i; j <= lim; j += i)
                if (a280862_spf[j] == 0) a280862_spf[j] = (uint32_t)i;
        }
    }
}

static bool a280862_test(uint64_t n) {
    if (n == 1) return true;
    uint64_t pr[20], ex[20];
    int c = 0;
    uint64_t m = n;
    while (m > 1) {
        uint64_t p = a280862_spf[m];
        uint64_t e = 0;
        while (m % p == 0) { m /= p; e++; }
        pr[c] = p; ex[c] = e; c++;
    }
    uint64_t phi = 1, lam = 1;
    for (int i = 0; i < c; i++) {
        uint64_t p = pr[i], e = ex[i];
        uint64_t pp = 1;
        for (uint64_t k = 1; k < e; k++) pp *= p;
        phi *= (p - 1) * pp;
        uint64_t l;
        if (p == 2) {
            if (e <= 2) l = (uint64_t)1 << (e - 1);
            else l = (uint64_t)1 << (e - 2);
        } else {
            l = (p - 1) * pp;
        }
        lam = lam / a280862_gcd(lam, l) * l;
    }
    uint64_t g;
    if ((n & 1) == 0) {
        g = 1;
    } else {
        static uint64_t divs[8192];
        size_t nd = 1;
        divs[0] = 1;
        for (int i = 0; i < c; i++) {
            size_t cur = nd;
            uint64_t pw = 1;
            for (uint64_t k = 1; k <= ex[i]; k++) {
                pw *= pr[i];
                for (size_t j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
            }
        }
        g = 0;
        for (size_t j = 0; j < nd; j++) {
            if (divs[j] > 1) g = a280862_gcd(g, divs[j] - 1);
        }
    }
    return g * lam == phi;
}

seq_t a(idx_t n) {
    if (a280862_lim == 0) a280862_sieve((uint64_t)1 << 16);
    while (a280862_cnt < (uint64_t)n) {
        uint64_t m = a280862_next++;
        if (m > a280862_lim) a280862_sieve(a280862_lim * 2);
        if (a280862_test(m)) {
            if (a280862_cnt == a280862_cap) {
                a280862_cap = a280862_cap ? a280862_cap * 2 : 1024;
                a280862_terms = (uint64_t *)realloc(a280862_terms, a280862_cap * sizeof(uint64_t));
            }
            a280862_terms[a280862_cnt++] = m;
        }
    }
    return (seq_t)a280862_terms[n - 1];
}
