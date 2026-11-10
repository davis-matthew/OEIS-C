/*
 * A347100  a(n) = phi(A003961(n)) - phi(n), where A003961 is the prime shift
 *          towards larger primes, and phi is Euler totient function.
 * https://oeis.org/A347100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(A003961(n)) - phi(n); phi(A003961(n)) is
 *            multiplicative with p^e -> (q-1)*q^(e-1), q = nextprime(p)
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 99 terms through n = 9927 (2026-11-10)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    seq_t shifted = 1;
    seq_t phi = 1;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            m /= p;
            shifted *= (seq_t)(q - 1);
            phi *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                shifted *= (seq_t)q;
                phi *= (seq_t)p;
            }
        }
    }
    if (m > 1) {
        uint64_t q = next_prime(m);
        shifted *= (seq_t)(q - 1);
        phi *= (seq_t)(m - 1);
    }
    return shifted - phi;
}
