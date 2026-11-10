/*
 * A277118  For a lesser p of twin primes, let B_k be A159559, but with
 *          initial term k; then a(n) is the smallest m such that
 *          B_(p+2)(m)-B_p(m)>6, where p = A001359(n-1), or a(n) = 0 if there
 *          is no such m.
 * https://oeis.org/A277118
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>=2 with B_{p+2}(m) - B_p(m) > 6, else 0, where p
 *            = A001359(n-1) and B_k(2) = k, B_k(j) = least x > B_k(j-1) with
 *            x prime iff j prime. Computed by direct simulation (by the
 *            theorem, m <= 17 whenever it exists, so the search is capped at
 *            40)
 * Range:     u8 n <= 5001, u16 n <= 5001, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Validated: standard, pass, 100 terms through n = 4686 (2026-11-08)
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

static uint64_t A001359_get(uint64_t n) {
    while ((uint64_t)A001359_cnt < n) {
        uint64_t nl = A001359_lim ? A001359_lim * 2 : 4096;
        A001359_extend(nl);
    }
    return A001359_tw[n - 1];
}

static bool A277118_small_prime(uint64_t m) {
    uint64_t d;
    if (m < 2) return false;
    for (d = 2; d * d <= m; d++)
        if (m % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    uint64_t p, x, y, m;
    if (n < 2) return (seq_t)0;
    p = A001359_get((uint64_t)n - 1);
    x = p;
    y = p + 2;
    for (m = 3; m <= 40; m++) {
        bool need = A277118_small_prime(m);
        x++;
        while (is_prime_u64(x) != need) x++;
        y++;
        while (is_prime_u64(y) != need) y++;
        if (y - x > 6) return (seq_t)m;
    }
    return (seq_t)0;
}
