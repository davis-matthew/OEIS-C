/*
 * A335128  Records in A006863.
 * https://oeis.org/A335128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th strict left-to-right maximum (record) of A006863(k),
 *            k = 0,1,2,...; computed by scanning A006863 and memoizing the
 *            records
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

static void A006863_mulp(seq_t *result, idx_t m, idx_t p) {
    idx_t b = 0;
    idx_t mc = m;
    while (mc % p == 0) {
        b++;
        mc /= p;
    }
    for (idx_t i = 0; i <= b; i++) {
        *result *= (seq_t)p;
    }
}

static seq_t A006863_a(idx_t n) {
    if (n == 0) return 1;

    idx_t m = 2 * n;
    seq_t result = 1;

    idx_t a2 = 0;
    idx_t m2 = m;
    while (m2 % 2 == 0) {
        a2++;
        m2 /= 2;
    }
    for (idx_t i = 0; i < a2 + 2; i++) {
        result *= 2;
    }

    for (idx_t d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            idx_t p = d + 1;
            if (p > 2 && is_prime_u64(p)) A006863_mulp(&result, m, p);
            idx_t d2 = m / d;
            if (d2 != d) {
                p = d2 + 1;
                if (p > 2 && is_prime_u64(p)) A006863_mulp(&result, m, p);
            }
        }
    }

    return result;
}

#define A335128_CAP 4096
static seq_t A335128_recs[A335128_CAP];
static idx_t A335128_cnt = 0;
static idx_t A335128_scan = 0;

seq_t a(idx_t n) {
    if (n == 0 || n > A335128_CAP) return 0;
    while (A335128_cnt < n) {
        seq_t v = A006863_a(A335128_scan);
        A335128_scan++;
        if (A335128_cnt == 0 || v > A335128_recs[A335128_cnt - 1]) {
            A335128_recs[A335128_cnt] = v;
            A335128_cnt++;
        }
    }
    return A335128_recs[n - 1];
}
