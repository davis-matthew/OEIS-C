/*
 * A347873  Numbers k such that k and A003415(sigma(k)) are of different
 *          parity.
 * https://oeis.org/A347873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003415(sigma(k)) mod 2 != k mod 2;
 *            found by a linear search over k
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A347873_sigma(uint64_t k) {
    uint64_t s = 1;
    for (uint64_t p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            uint64_t t = 1, pw = 1;
            while (k % p == 0) {
                k /= p;
                pw *= p;
                t += pw;
            }
            s *= t;
        }
    }
    if (k > 1) s *= (k + 1);
    return s;
}

/* parity of arithmetic derivative of s */
static int A347873_dpar(uint64_t s) {
    if (s < 2) return 0;
    int v2 = 0;
    while ((s & 1) == 0) { s >>= 1; v2++; }
    if (v2 > 0) return v2 == 1 ? 1 : 0;
    int cnt = 0;
    for (uint64_t p = 3; p * p <= s; p += 2) {
        while (s % p == 0) { s /= p; cnt++; }
    }
    if (s > 1) cnt++;
    return cnt & 1;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A347873_dpar(A347873_sigma(k)) != (int)(k & 1)) count++;
    }
    return (seq_t)k;
}
