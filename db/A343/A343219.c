/*
 * A343219  a(n) = 1 if A003415(sigma(k)) > k, otherwise 0.
 * https://oeis.org/A343219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003415(sigma(n)) > n, else 0; sigma = A000203,
 *            A003415 = arithmetic derivative
 * Range:     u8 n <= 41, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9145 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A003415_a(seq_t n) {
    if (n == 0 || n == 1)
        return 0;
    seq_t result = 0;
    seq_t remaining = n;
    for (seq_t p = 2; p * p <= remaining; p++) {
        int exponent = 0;
        while (remaining % p == 0) {
            exponent++;
            remaining /= p;
        }
        if (exponent > 0) {
            result += (n / p) * (seq_t)exponent;
        }
    }
    if (remaining > 1) {
        result += n / remaining;
    }
    return result;
}

static seq_t A343219_sigma(seq_t m) {
    seq_t res = 1;
    seq_t rem = m;
    for (seq_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            seq_t term = 1, pk = 1;
            while (rem % p == 0) {
                rem /= p;
                pk *= p;
                term += pk;
            }
            res *= term;
        }
    }
    if (rem > 1) {
        res *= (rem + 1);
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t k = (seq_t)n;
    seq_t s = A343219_sigma(k);
    seq_t d = A003415_a(s);
    return (d > k) ? (seq_t)1 : (seq_t)0;
}
