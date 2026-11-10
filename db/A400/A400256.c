/*
 * A400256  Numbers where the iteration of map k -> A003415(k) leads to a
 *          prime (and thus to 1 and 0) in the shortest possible time, with
 *          the maximal exponent of k (A051903) decreasing by 1 on each
 *          iteration until a prime is encountered.
 * https://oeis.org/A400256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 2 such that, with e = A051903(k), either k is
 *            prime or the iterates k_i = A003415^i(k) have A051903(k_i) = e-i
 *            for i < e and k_e is prime; found by a search with memoized
 *            progress
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9290 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static bool A400256_good(uint64_t k) {
    uint64_t pr[64], ex[64];
    size_t cnt;
    uint64_t cur = k;
    int expected = -1;
    for (;;) {
        if (cur < 2) return false;
        cnt = 0;
        factor_u64(cur, pr, ex, &cnt);
        uint64_t mx = 0, d = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (ex[i] > mx) mx = ex[i];
            d += (cur / pr[i]) * ex[i];
        }
        bool prime = (cnt == 1 && ex[0] == 1);
        if (expected < 0) expected = (int)mx;
        if (prime) return expected <= 1;
        if (expected < 1 || (int)mx != expected) return false;
        expected--;
        cur = d;
    }
}

seq_t a(idx_t n) {
    static uint64_t last_n = 0, last_k = 1;
    if (n < last_n || last_n == 0) {
        last_n = 0;
        last_k = 1;
    }
    uint64_t cnt = last_n;
    uint64_t k = last_k;
    while (cnt < (uint64_t)n) {
        ++k;
        if (A400256_good(k)) ++cnt;
    }
    last_n = cnt;
    last_k = k;
    return (seq_t)k;
}
