/*
 * A348994  a(n) = A003961(n) / gcd(n, A003961(n)), where A003961(n) is fully
 *          multiplicative with a(prime(k)) = prime(k+1).
 * https://oeis.org/A348994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A003961(n) / gcd(n, A003961(n))
 * Range:     u8 n <= 47, u16 n <= 1791, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9089 (2026-11-01)
 * Helpers:   gcd, is_prime_seq, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"

static seq_t next_prime(uint64_t p) {
    uint64_t q = p + 1;
    while (!is_prime_seq(q)) ++q;
    return (seq_t)q;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t tmp = n;          /* make a copy of n for factorization */
    seq_t product = 1;      /* A003961(n) */

    while (tmp > 1) {
        uint64_t prime = smallest_prime_factor(tmp);
        idx_t exp = 0;
        while (tmp % prime == 0) {
            tmp /= prime;
            ++exp;
        }

        seq_t next = next_prime(prime);
        seq_t pow = 1;
        for (idx_t i = 0; i < exp; ++i) pow *= next;
        product *= pow;
    }

    seq_t g = gcd((seq_t)n, product);
    return product / g;
}
