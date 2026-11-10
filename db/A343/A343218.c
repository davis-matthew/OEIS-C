/*
 * A343218  Numbers k such that A003415(sigma(k)) > k.
 * https://oeis.org/A343218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A003415(sigma(k)) > k; found by
 *            direct search over k
 * Range:     u8 n <= 28, u16 n <= 5830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9911 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A003415_a(seq_t m) {
    if (m < 2) return 0;
    seq_t result = 0;
    seq_t remaining = m;
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) result += (m / p) * (seq_t)exponent;
    }
    if (remaining > 1) result += m / remaining;
    return result;
}

static seq_t sigma_local(seq_t k) {
    seq_t res = 1;
    seq_t rem = k;
    for (seq_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            seq_t term = 1, pw = 1;
            while (rem % p == 0) {
                rem /= p;
                pw *= p;
                term += pw;
            }
            res *= term;
        }
    }
    if (rem > 1) res *= (rem + 1);
    return res;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (count < n) {
        k++;
        if (A003415_a(sigma_local(k)) > k) count++;
    }
    return k;
}
