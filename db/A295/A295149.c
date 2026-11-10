/*
 * A295149  Numbers n such that gcd(A002487(n), A002487(n+2)) > 1.
 * https://oeis.org/A295149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k (odd) with gcd(A002487(k), A002487(k+2)) >
 *            1; computed by search, A002487 via fast binary iteration
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A295149_stern(idx_t n) {
    seq_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x; else x += y;
        n >>= 1;
    }
    return y;
}

static seq_t A295149_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

#define A295149_CAP 100000
static idx_t A295149_cache[A295149_CAP];
static idx_t A295149_count = 0;
static idx_t A295149_next = 1;

seq_t a(idx_t n) {
    while (A295149_count < n && A295149_count < A295149_CAP) {
        idx_t k = A295149_next;
        A295149_next += 2;  /* all terms are odd */
        if (A295149_gcd(A295149_stern(k), A295149_stern(k + 2)) > 1)
            A295149_cache[A295149_count++] = k;
    }
    if (n <= A295149_count) return (seq_t)A295149_cache[n - 1];
    /* beyond cache: continue scan without storing */
    idx_t cnt = A295149_count;
    idx_t k = A295149_next;
    for (;; k += 2) {
        if (A295149_gcd(A295149_stern(k), A295149_stern(k + 2)) > 1) {
            if (++cnt == n) return (seq_t)k;
        }
    }
}
