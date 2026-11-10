/*
 * A392869  Numbers k for which A003415(k) >= A276085(k), where A003415 is the
 *          arithmetic derivative and A276085 is the primorial base
 *          log-function.
 * https://oeis.org/A392869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A003415(k) >= A276085(k); found by search
 *            (increasing k, trial division, with an early cutoff once the
 *            primorial sum exceeds k')
 * Range:     u8 n <= 54, u16 n <= 991, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9915 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A392869_small_prime(uint64_t q) {
    if (q < 2) return false;
    for (uint64_t d = 2; d * d <= q; d++)
        if (q % d == 0) return false;
    return true;
}

static bool A392869_test(uint64_t k) {
    if (k <= 1) return true;
    seq_t kk = (seq_t)k;
    uint64_t rem = k;
    seq_t d = 0;
    uint64_t ps[64];
    unsigned es[64];
    int cnt = 0;
    for (uint64_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            d += (kk / (seq_t)p) * (seq_t)e;
            ps[cnt] = p; es[cnt] = e; cnt++;
        }
    }
    if (rem > 1) {
        d += kk / (seq_t)rem;
        ps[cnt] = rem; es[cnt] = 1; cnt++;
    }
    seq_t sum = 0;
    for (int i = 0; i < cnt; i++) {
        seq_t P = 1;
        for (uint64_t q = 2; q < ps[i]; q++) {
            if (!A392869_small_prime(q)) continue;
            if (P > d / (seq_t)q) return false; /* P*q > d */
            P *= (seq_t)q;
        }
        if (P > d / (seq_t)es[i]) {
            if (P * (seq_t)es[i] / (seq_t)es[i] != P || P * (seq_t)es[i] > d) return false;
        }
        sum += P * (seq_t)es[i];
        if (sum > d) return false;
    }
    return d >= sum;
}

seq_t a(idx_t n) {
    static uint64_t last_n = 0, last_k = 0;
    uint64_t cnt, k;
    if (last_n != 0 && (uint64_t)n >= last_n) {
        cnt = last_n; k = last_k;
    } else {
        cnt = 0; k = 0;
    }
    while (cnt < (uint64_t)n) {
        k++;
        if (A392869_test(k)) cnt++;
    }
    last_n = cnt; last_k = k;
    return (seq_t)k;
}
