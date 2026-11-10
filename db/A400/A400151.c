/*
 * A400151  a(n) = A328571(A003415(n)).
 * https://oeis.org/A400151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A328571(A003415(n)): take the arithmetic derivative n',
 *            then the product of the primes p_i whose primorial-base digit of
 *            n' is nonzero
 * Range:     u8 n <= 143, u16 n <= 6143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 8816 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A328571_a(seq_t m) {
    seq_t result = 1;
    seq_t p = 2;
    while (m != 0) {
        if (m % p != 0) result *= p;
        m /= p;
        /* advance p to the next prime */
        do {
            p++;
            bool pr = true;
            for (seq_t q = 2; q * q <= p; q++) {
                if (p % q == 0) { pr = false; break; }
            }
            if (pr) break;
        } while (true);
    }
    return result;
}

static seq_t A003415_a(idx_t n) {
    if (n < 2) return 0;
    seq_t orig = (seq_t)n;
    seq_t rem = orig;
    seq_t res = 0;
    for (seq_t p = 2; p * p <= rem; p++) {
        int e = 0;
        while (rem % p == 0) { e++; rem /= p; }
        if (e > 0) res += (orig / p) * (seq_t)e;
    }
    if (rem > 1) res += orig / rem;
    return res;
}

seq_t a(idx_t n) {
    return A328571_a(A003415_a(n));
}
