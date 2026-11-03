/*
 * A333748  Number of nonprime divisors of n that are <= sqrt(n).
 * https://oeis.org/A333748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   is_prime_seq, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = the number of non‑prime divisors of n that are <= floor(sqrt(n)). */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/isqrt.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t limit = isqrt(n);
    seq_t count = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            /* d is a divisor; count it if it is not prime */
            if (!is_prime_seq((seq_t)d)) {
                ++count;
            }
        }
    }
    return count;
}
