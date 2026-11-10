/*
 * A364559  a(n) = A005941(n) - n.
 * https://oeis.org/A364559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A005941(n) - n, where A005941(n) = 1 + Sum_{j=1..k}
 *            2^(i_j - 1 + j - 1) for n = prime(i_1)*...*prime(i_k), i_1 <=
 *            ... <= i_k
 * Range:     i8 n <= 22, i16 n <= 58, i32 n <= 136, i64 n <= 312,
 *            i128 n <= 726, big any n
 * Validated: standard, pass, 88 terms through n = 9773 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A364559_pi_tab = NULL;
static uint64_t A364559_pi_cap = 0;

static void A364559_ensure_sieve(uint64_t lim) {
    if (lim < A364559_pi_cap) return;
    uint64_t cap = A364559_pi_cap ? A364559_pi_cap : 1024;
    while (cap <= lim) cap *= 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)cap, 1);
    uint32_t *tab = (uint32_t *)malloc((size_t)cap * sizeof(uint32_t));
    comp[0] = 1;
    if (cap > 1) comp[1] = 1;
    for (uint64_t i = 2; i * i < cap; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j < cap; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i < cap; i++) {
        if (!comp[i]) c++;
        tab[i] = c;
    }
    free(comp);
    free(A364559_pi_tab);
    A364559_pi_tab = tab;
    A364559_pi_cap = cap;
}

static seq_t A364559_p2(uint64_t e) {
    seq_t r = 1;
    for (uint64_t i = 0; i < e; i++) r = r * 2;
    return r;
}

seq_t a(idx_t n) {
    A364559_ensure_sieve(n);
    seq_t s = 0;
    uint64_t m = n;
    uint64_t j = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p != 0) continue;
        uint64_t idx = A364559_pi_tab[p];
        while (m % p == 0) {
            m /= p;
            s += A364559_p2(idx - 1 + j);
            j++;
        }
    }
    if (m > 1) {
        uint64_t idx = A364559_pi_tab[m];
        s += A364559_p2(idx - 1 + j);
    }
    return s + 1 - (seq_t)n;
}
