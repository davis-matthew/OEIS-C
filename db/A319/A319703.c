/*
 * A319703  a(n) = A003415(A252463(n)).
 * https://oeis.org/A319703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A252463(n)); A252463(1)=1, A252463(2n)=n,
 *            A252463(2n+1)=prod prev_prime(p)^e over p^e || 2n+1; A003415 =
 *            arithmetic derivative
 * Range:     u8 n <= 191, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 110 terms through n = 9672 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A319703_isprime(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A319703_prevprime(uint64_t n) {
    uint64_t p = n - 1;
    while (!A319703_isprime(p)) --p;
    return p;
}

static seq_t A003415_a(seq_t n) {
    if (n == 0 || n == 1) return 0;
    seq_t result = 0;
    seq_t rem = n;
    for (seq_t p = 2; p <= rem / p; p++) {
        int e = 0;
        while (rem % p == 0) { e++; rem /= p; }
        if (e > 0) result += (n / p) * (seq_t)e;
    }
    if (rem > 1) result += n / rem;
    return result;
}

static seq_t A252463_a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if ((n & 1u) == 0u) return (seq_t)(n >> 1);
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            seq_t base = (seq_t)A319703_prevprime(p);
            while (m % p == 0) { m /= p; result *= base; }
        }
    }
    if (m > 1) result *= (seq_t)A319703_prevprime(m);
    return result;
}

seq_t a(idx_t n) {
    return A003415_a(A252463_a(n));
}
