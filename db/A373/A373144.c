/*
 * A373144  Numbers k such that both A003415(k) and A276085(k) are multiples
 *          of 3, where A003415 is the arithmetic derivative and A276085 is
 *          the primorial base log-function.
 * https://oeis.org/A373144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with A003415(k) == 0 (mod 3) and A276085(k)
 *            == 0 (mod 3); A276085(k) == e2 - e3 (mod 3); found by a search
 *            over k with trial division
 * Range:     u8 n <= 25, u16 n <= 9128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 7528 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A373144_ok(uint64_t k) {
    if (k < 2) return true;
    uint64_t m = k;
    int e2 = 0, e3 = 0;
    while ((m & 1ULL) == 0) { e2++; m >>= 1; }
    while (m % 3ULL == 0) { e3++; m /= 3ULL; }
    if ((((e2 - e3) % 3) + 3) % 3 != 0) return false;

    /* arithmetic derivative mod 3 */
    uint64_t d = 0;
    if (e2) d += ((k / 2) % 3) * (uint64_t)e2;
    if (e3) d += ((k / 3) % 3) * (uint64_t)e3;
    for (uint64_t p = 5; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            d += ((k / p) % 3) * (uint64_t)e;
        }
    }
    if (m > 1) d += (k / m) % 3;
    return d % 3 == 0;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A373144_ok(k)) count++;
    }
    return (seq_t)k;
}
