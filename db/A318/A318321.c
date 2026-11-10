/*
 * A318321  Numerators of rational valued sequence whose Dirichlet convolution
 *          with itself yields A003961.
 * https://oeis.org/A318321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = Product over p^k || n of oddpart(binomial(2k,k)) *
 *            nextprime(p)^k (f = (1-q x)^(-1/2) per prime power, q = next
 *            prime after p; denominators are powers of 2)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 2047, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 87 terms through n = 6377 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A318321_cb_odd(unsigned k) {
    seq_t c = 1;
    for (unsigned i = 1; i <= k; i++) {
        c = c * (seq_t)(k + i) / (seq_t)i;
    }
    while (c > 0 && (c % 2) == 0) c /= 2;
    return c;
}

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned k = 0;
            while (m % p == 0) { m /= p; k++; }
            seq_t q = (seq_t)next_prime(p);
            result *= A318321_cb_odd(k);
            for (unsigned i = 0; i < k; i++) result *= q;
        }
    }
    if (m > 1) {
        result *= (seq_t)next_prime(m);
    }
    return result;
}
