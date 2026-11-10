/*
 * A373147  a(n) = A003415(n) mod A276085(n), where A003415 is the arithmetic
 *          derivative and A276085 is the primorial base log-function.
 * https://oeis.org/A373147
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(n) mod A276085(n), where A003415(n) = Sum_{p^e ||
 *            n} e*n/p and A276085(n) = Sum_{p^e || n} e*(product of primes <
 *            p)
 * Range:     u8 n <= 25, u16 n <= 1263, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 109 terms through n = 8813 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a373147_isprime(uint64_t k) {
    if (k < 2) return false;
    for (uint64_t d = 2; d * d <= k; d++)
        if (k % d == 0) return false;
    return true;
}

static seq_t a373147_primorial_below(uint64_t p) {
    seq_t prod = 1;
    for (uint64_t k = 2; k < p; k++)
        if (a373147_isprime(k)) prod = prod * (seq_t)k;
    return prod;
}

seq_t a(idx_t n) {
    uint64_t rem = (uint64_t)n;
    seq_t N = (seq_t)n;
    seq_t D = 0, L = 0;
    for (uint64_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            unsigned e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            D += (N / (seq_t)p) * (seq_t)e;
            L += (seq_t)e * a373147_primorial_below(p);
        }
    }
    if (rem > 1) {
        D += N / (seq_t)rem;
        L += a373147_primorial_below(rem);
    }
    if (L == 0) return 0;
    return D % L;
}
