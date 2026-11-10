/*
 * A247897  First differences of A247676.
 * https://oeis.org/A247897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A247676(n+1) - A247676(n), where A247676 lists the odd
 *            composite numbers == 2 (mod 9), i.e. composites of the form
 *            18*t+11; computed by a scan with a primality test
 * Range:     u8 n <= 4, u16 n <= 4858, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9978 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t count = 0;
    seq_t first = 0;
    idx_t t = 0;
    for (;;) {
        seq_t cand = (seq_t)18 * (seq_t)t + (seq_t)11;
        ++t;
        if (is_prime_u64((uint64_t)cand)) continue;
        ++count;
        if (count == n) {
            first = cand;
        } else if (count == n + 1) {
            return cand - first;
        }
    }
}
