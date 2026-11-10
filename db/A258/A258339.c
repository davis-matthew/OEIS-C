/*
 * A258339  PrimePi(A258337(n)).
 * https://oeis.org/A258339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = PrimePi(A258337(n)), where A258337(n) is the smallest
 *            prime not already used whose decimal expansion begins with the
 *            digits of n (computed by search)
 * Range:     u8 n <= 19, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 102 terms through n = 4400 (2026-11-07)
 * Helpers:   is_prime_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/prime_pi.h"

static uint64_t *ps_arr = 0;
static size_t ps_cnt = 0, ps_cap = 0;

static bool ps_used(uint64_t p) {
    for (size_t i = 0; i < ps_cnt; i++)
        if (ps_arr[i] == p) return true;
    return false;
}

static uint64_t ps_next(uint64_t n) {
    uint64_t scale = 1;
    for (;;) {
        uint64_t lo = n * scale, hi = (n + 1) * scale - 1;
        for (uint64_t m = lo; m <= hi; m++) {
            if (m >= 2 && is_prime_u64(m) && !ps_used(m)) return m;
        }
        scale *= 10;
    }
}

seq_t a(idx_t n) {
    while (ps_cnt < (size_t)n) {
        if (ps_cnt == ps_cap) {
            ps_cap = ps_cap ? ps_cap * 2 : 1024;
            ps_arr = (uint64_t *)realloc(ps_arr, ps_cap * sizeof(uint64_t));
        }
        uint64_t p = ps_next((uint64_t)ps_cnt + 1);
        ps_arr[ps_cnt++] = p;
    }
    return prime_pi((idx_t)ps_arr[n - 1]);
}
