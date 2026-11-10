/*
 * A296208  Xor-Moebius transform of A005187.
 * https://oeis.org/A296208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR_{d|n, n/d squarefree} A005187(d), where A005187(m) =
 *            Sum_{k>=0} floor(m/2^k)
 * Range:     u8 n <= 128, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 92 terms through n = 9694 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A005187_val(idx_t n) {
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t pf[20];
    int cnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            pf[cnt++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) pf[cnt++] = t;

    seq_t res = 0;
    uint64_t total = (uint64_t)1 << cnt;
    for (uint64_t mask = 0; mask < total; ++mask) {
        idx_t m = 1;
        for (int j = 0; j < cnt; ++j) {
            if (mask & ((uint64_t)1 << j)) m *= pf[j];
        }
        res ^= A005187_val(n / m);
    }
    return res;
}
