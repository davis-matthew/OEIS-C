/*
 * A339020  Largest value of (p*q mod n), for primes p and q, where p + q = n
 *          and p <= q (or 0 if no such primes exist).
 * https://oeis.org/A339020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute largest p*q mod n for primes p+q=n (p <= q). Return 0 if no such primes exist. */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t max = 0;
    for (idx_t p = 2; p <= n / 2; ++p) {
        if (is_prime(p) && is_prime(n - p)) {
            seq_t prod = (seq_t)p * (seq_t)(n - p);
            seq_t val = prod % (seq_t)n;
            if (val > max) {
                max = val;
            }
        }
    }
    return max;
}
