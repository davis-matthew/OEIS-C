/*
 * A324669  a(n) is the least k>0 such that A001359(n)+k^2 is in A001359.
 * https://oeis.org/A324669
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least k>0 (k a multiple of 6) such that p+k^2 and
 *            p+k^2+2 are both prime, where p = A001359(n)
 * Range:     u8 n <= 6, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9397 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A001359_tw = NULL;
static size_t A001359_cnt = 0;
static uint64_t A001359_lim = 0;

static void A001359_extend(uint64_t newlim) {
    unsigned char *comp = (unsigned char *)malloc((size_t)(newlim + 3));
    uint64_t i, j, top = newlim + 2;
    size_t cap = 1024, c = 0;
    uint64_t *list;
    memset(comp, 0, (size_t)(newlim + 3));
    comp[0] = comp[1] = 1;
    for (i = 2; i <= top / i; i++)
        if (!comp[i])
            for (j = i * i; j <= top; j += i) comp[j] = 1;
    list = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (i = 2; i <= newlim; i++) {
        if (!comp[i] && !comp[i + 2]) {
            if (c == cap) {
                cap *= 2;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[c++] = i;
        }
    }
    free(comp);
    free(A001359_tw);
    A001359_tw = list;
    A001359_cnt = c;
    A001359_lim = newlim;
}

static uint64_t A001359_get(idx_t n) {
    while ((uint64_t)A001359_cnt < (uint64_t)n) {
        uint64_t nl = A001359_lim ? A001359_lim * 2 : 4096;
        A001359_extend(nl);
    }
    return A001359_tw[n - 1];
}

static bool A324669_small_ok(uint64_t m) {
    static const unsigned sp[] = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};
    size_t i;
    for (i = 0; i < sizeof(sp) / sizeof(sp[0]); i++) {
        uint64_t r = m % sp[i];
        if (r == 0 || (r + 2) % sp[i] == 0) {
            if (m > (uint64_t)sp[i] + 2) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t p = A001359_get(n);
    uint64_t k;
    for (k = 6;; k += 6) {
        uint64_t m = p + k * k;
        if (!A324669_small_ok(m)) continue;
        if (is_prime_u64(m) && is_prime_u64(m + 2)) return (seq_t)k;
    }
}
