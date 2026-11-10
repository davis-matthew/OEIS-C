/*
 * A354874  a(n) = 1 if A003415(n) is squarefree, otherwise 0.
 * https://oeis.org/A354874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 0; for n > 1, a(n) = 1 if the arithmetic
 *            derivative n' (A003415) is squarefree, else 0
 * Range:     u8 n <= 1434, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 138 terms through n = 9915 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A003415_a(idx_t n) {
    if (n == 0 || n == 1)
        return 0;
    seq_t result = 0;
    seq_t original_n = (seq_t)n;
    seq_t remaining = original_n;
    for (seq_t p = 2; p <= remaining / p; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (original_n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1) {
        result += original_n / remaining;
    }
    return result;
}

static bool A354874_squarefree(seq_t m) {
    if (m < 2)
        return true;
    seq_t t = m;
    for (seq_t i = 2; i <= t / i; ++i) {
        if (t % i == 0) {
            int e = 0;
            do {
                t /= i;
                ++e;
            } while (t % i == 0);
            if (e > 1)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    if (n < 2)
        return 0;
    seq_t d = A003415_a(n);
    return A354874_squarefree(d) ? (seq_t)1 : (seq_t)0;
}
