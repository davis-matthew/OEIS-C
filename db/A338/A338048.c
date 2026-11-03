/*
 * A338048  a(n) is the smallest number that is not less than n such that
 *          n*a(n)+2 is semiprime.
 * https://oeis.org/A338048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = min{ a ≥ n | n*a + 2 is a semiprime } */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega_seq.h"

/* Helper to test for semiprimality (product of exactly two primes,
   allowing repetition). */
static inline bool is_semiprime(seq_t x) {
    return bigomega_seq(x) == 2;
}

/* Sequence function. */
seq_t a(idx_t n) {
    seq_t a_val = (seq_t)n;          /* start from n */
    while (true) {
        seq_t product = (seq_t)n * a_val + (seq_t)2;
        if (is_semiprime(product)) {
            return a_val;
        }
        ++a_val;
    }
}
