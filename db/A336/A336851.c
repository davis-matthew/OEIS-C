/*
 * A336851  a(n) = sigma(A003961(n)) - A003961(n), where A003961 is a prime
 *          shift towards larger primes, sigma is the sum of divisors.
 * https://oeis.org/A336851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(A003961(n)) - A003961(n); multiplicative sigma with
 *            p^e -> q^e, q = nextprime(p), minus A003961(n)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 98 terms through n = 9396 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A336851_a(idx_t n) {
    seq_t sig = 1;
    seq_t shifted = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            seq_t term = 1;
            seq_t pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)q;
                term += pw;
            }
            sig *= term;
            shifted *= pw;
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        sig *= ((seq_t)q + 1);
        shifted *= (seq_t)q;
    }
    return sig - shifted;
}

seq_t a(idx_t n) {
    return A336851_a(n);
}
