/*
 * A328112  a(1) = 0, for primes p, a(p) = 1, and for any other number n, a(n)
 *          = max(A003415(n), A276086(n)).
 * https://oeis.org/A328112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(p)=1 for primes p, else a(n) = max(A003415(n),
 *            A276086(n))
 * Range:     u8 n <= 19, u16 n <= 85, u32 n <= 416, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 93 terms through n = 2102 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A328112_isprime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p = 2; p * p <= n; p++)
        if (n % p == 0) return false;
    return true;
}

static seq_t A328112_deriv(uint64_t n) {
    seq_t result = 0;
    uint64_t rem = n;
    for (uint64_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        if (e > 0) result += (seq_t)(n / p) * (seq_t)e;
    }
    if (rem > 1) result += (seq_t)(n / rem);
    return result;
}

static seq_t A328112_primorial_exp(uint64_t n) {
    seq_t result = 1;
    uint64_t p = 2;
    while (n > 0) {
        uint64_t d = n % p;
        n /= p;
        for (uint64_t i = 0; i < d; i++) result *= (seq_t)p;
        p++;
        while (!A328112_isprime(p)) p++;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    if (A328112_isprime((uint64_t)n)) return 1;
    seq_t x = A328112_deriv((uint64_t)n);
    seq_t y = A328112_primorial_exp((uint64_t)n);
    return x > y ? x : y;
}
