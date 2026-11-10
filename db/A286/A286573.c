/*
 * A286573  Compound filter: a(n) = P(A007733(n), A046523(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + (x+k)^2 - x - 3*k), x = A007733(n), k =
 *            A046523(n)
 * Range:     u8 n <= 17, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 7806 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A007733_a(idx_t n) {
    idx_t m = n;
    while (m % 2 == 0) m /= 2;
    if (m == 1) return (seq_t)1;
    idx_t power = 2 % m;
    idx_t order = 1;
    while (power != 1) {
        power = (power * 2) % m;
        order++;
    }
    return (seq_t)order;
}

static seq_t A046523_a(idx_t n) {
    int ex[70];
    int cnt = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            ex[cnt++] = e;
        }
    }
    if (m > 1) ex[cnt++] = 1;
    /* sort descending (insertion sort) */
    for (int i = 1; i < cnt; i++) {
        int v = ex[i];
        int j = i - 1;
        while (j >= 0 && ex[j] < v) { ex[j + 1] = ex[j]; j--; }
        ex[j + 1] = v;
    }
    static const int pr[16] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53};
    seq_t r = 1;
    for (int i = 0; i < cnt; i++) {
        for (int e = 0; e < ex[i]; e++) r *= (seq_t)pr[i];
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t x = A007733_a(n);
    seq_t k = A046523_a(n);
    seq_t s = x + k;
    seq_t t = s * s + 2;
    t -= x;
    t -= 3 * k;
    return t / 2;
}
