/*
 * A348740  Positions k where A348733(k) is not multiplicative.
 * https://oeis.org/A348740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (in increasing order) such that for some coprime
 *            split k = x*(k/x), A348733(x)*A348733(k/x) <> A348733(k), where
 *            A348733(m) = gcd(A003959(m), A034448(m)); found by a search over
 *            k, testing all coprime splits via prime-power parts
 * Range:     u8 none, u16 n <= 1, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static uint64_t *nm_terms = NULL;
static size_t nm_count = 0;
static size_t nm_cap = 0;
static uint64_t nm_next_k = 2;

static bool nm_is_nonmult(uint64_t k) {
    seq_t pa[20], pb[20];
    int r = 0;
    uint64_t t = k;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p) continue;
        seq_t A = 1, B = 1, pw = 1;
        while (t % p == 0) {
            t /= p;
            A *= (seq_t)(p + 1);
            pw *= (seq_t)p;
        }
        B = pw + (seq_t)1;
        pa[r] = A; pb[r] = B; ++r;
    }
    if (t > 1) {
        pa[r] = (seq_t)(t + 1);
        pb[r] = (seq_t)(t + 1);
        ++r;
    }
    if (r < 2) return false;
    seq_t TA = 1, TB = 1;
    for (int i = 0; i < r; ++i) { TA *= pa[i]; TB *= pb[i]; }
    seq_t fk = gcd(TA, TB);
    /* subsets containing prime 0, proper */
    uint32_t total = (uint32_t)1 << (r - 1);
    for (uint32_t m = 0; m + 1 < total; ++m) {
        seq_t SA = pa[0], SB = pb[0];
        for (int i = 1; i < r; ++i) {
            if ((m >> (i - 1)) & 1U) { SA *= pa[i]; SB *= pb[i]; }
        }
        seq_t CA = 1, CB = 1;
        for (int i = 1; i < r; ++i) {
            if (!((m >> (i - 1)) & 1U)) { CA *= pa[i]; CB *= pb[i]; }
        }
        seq_t f1 = gcd(SA, SB);
        seq_t f2 = gcd(CA, CB);
        if (f1 * f2 != fk) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (nm_count < (size_t)n) {
        uint64_t k = nm_next_k++;
        if (nm_is_nonmult(k)) {
            if (nm_count == nm_cap) {
                size_t nc = nm_cap ? nm_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(nm_terms, nc * sizeof(uint64_t));
                if (!p) return 0;
                nm_terms = p;
                nm_cap = nc;
            }
            nm_terms[nm_count++] = k;
        }
    }
    return (seq_t)nm_terms[(size_t)n - 1];
}
