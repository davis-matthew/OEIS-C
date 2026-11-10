/*
 * A358221  Numbers k such that A003415(k) divides A276086(k), where A003415
 *          is the arithmetic derivative, and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A358221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 2 such that A003415(k) divides A276086(k);
 *            found by search, testing A276086(k) mod k' with k' = A003415(k)
 * Range:     u8 n <= 75, u16 n <= 7184, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9341 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A358221_deriv(uint64_t n) {
    if (n < 2) return 0;
    uint64_t res = 0, rem = n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            res += (n / p) * (uint64_t)e;
        }
    }
    if (rem > 1) res += n / rem;
    return res;
}

static bool A358221_test(uint64_t k) {
    static const uint64_t pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t d = A358221_deriv(k);
    if (d == 0) return false;
    if (d == 1) return true;
    unsigned __int128 r = 1 % d;
    uint64_t m = k;
    for (int i = 0; m > 0 && i < 16; i++) {
        uint64_t dig = m % pr[i];
        m /= pr[i];
        for (uint64_t j = 0; j < dig; j++)
            r = (r * (pr[i] % d)) % d;
    }
    return r == 0;
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 2;
    while (cnt < (size_t)n) {
        if (A358221_test(next_k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[(size_t)n - 1];
}
