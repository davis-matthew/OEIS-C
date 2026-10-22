/*
 * A305808  Dirichlet convolution of A032742 (the largest proper divisor of n)
 *          with itself.
 * https://oeis.org/A305808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A032742(n/d)*A032742(d)
 * Range:     u8 n <= 95, u16 n <= 5039, u32 n <= 9402, u64 n <= 9402,
 *            u128 n <= 9402, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum over all divisors d of n of the product of the largest
 * proper divisors of d and n/d, with the largest proper divisor of 1
 * taken as 1.  A032742(m) = 1 if m is prime, else m / spf(m). */
/* Implementation uses trial division to find the smallest prime factor
 * of a number and enumerates all divisors in O(sqrt(n)). */

#include <stdint.h>

/* Smallest prime factor of a 64‑bit number (returns the number itself
 * if it is prime or 1).  Assumes n >= 1. */
static inline idx_t _spf_uint64(idx_t n) {
    if (n <= 1) return n;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) return p;
    }
    return n;  /* n is prime */
}

/* Largest proper divisor of m.  For m = 1 returns 1. */
static inline seq_t _lpd(idx_t m) {
    if (m <= 1) return (seq_t)1;
    idx_t spf = _spf_uint64(m);
    return (spf == m) ? (seq_t)1 : (seq_t)(m / spf);
}

/* Main sequence function.  Indexing starts at 1. */
seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t l1 = _lpd(d);
            seq_t l2 = _lpd(n / d);
            res += l1 * l2;
            if (d != n / d) res += l1 * l2;  /* add the symmetric divisor */
        }
    }
    return res;
}
