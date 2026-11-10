/*
 * A371152  Array read by antidiagonals: row n lists numbers m >= 2 with
 *          A362333(m) = n.
 * https://oeis.org/A371152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   table read by antidiagonals (T(n,k) with n increasing, k
 *            decreasing); T(n,k) = k-th m >= 2 with A362333(m) = n, where
 *            A362333(m) = max over p^e || m of ceil(e / v_p(gpf(m)!)); found
 *            by a scan, T(n,1) = 2^n
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A371152_val(uint64_t m) {
    uint64_t exps[64], primes[64];
    size_t cnt = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (t > 1) { primes[cnt] = t; exps[cnt] = 1; ++cnt; }
    uint64_t g = primes[cnt - 1];
    uint64_t best = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i], v = 0, q = g;
        while (q > 0) { q /= p; v += q; }
        uint64_t ki = (exps[i] + v - 1) / v;
        if (ki > best) best = ki;
    }
    return best;
}

seq_t a(idx_t n) {
    uint64_t d = 1;
    while ((d * (d + 1)) / 2 < (uint64_t)n) ++d;
    uint64_t idx = (uint64_t)n - (d * (d - 1)) / 2;
    uint64_t row = idx;
    uint64_t k = d + 1 - idx;
    if (k == 1) {
        seq_t r = 1;
        for (uint64_t i = 0; i < row; ++i) r = r * 2;
        return r;
    }
    uint64_t count = 0;
    for (uint64_t m = 2;; ++m) {
        if (A371152_val(m) == row) {
            ++count;
            if (count == k) return (seq_t)m;
        }
    }
}
