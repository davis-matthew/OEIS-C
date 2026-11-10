/*
 * A246270  Number of prime factors of the form 4k+3 (counted with
 *          multiplicity) in A003961(n): a(n) = A065339(A003961(n)).
 * https://oeis.org/A246270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A065339(A003961(n)); totally additive, a(p) = 1 if
 *            nextprime(p) == 3 (mod 4), else 0
 * Range:     u8 n <= 10080, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 112 terms through n = 9895 (2026-11-07)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t count = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            if (next_prime(p) % 4 == 3) count += e;
        }
    }
    if (m > 1) {
        if (next_prime(m) % 4 == 3) count++;
    }
    return (seq_t)count;
}
