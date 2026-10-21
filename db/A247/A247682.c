/*
 * A247682  Odd composite numbers congruent to 5 modulo 9.
 * https://oeis.org/A247682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 2547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd composite numbers ≡ 5 (mod 9).  The function returns
   the n‑th term (offset 1; a(1) = 77).  It scans all numbers of the
   form 18·k + 5, counts those that are composite, and returns the nth
   composite.  The helper is_prime_small from the utils library is used
   for primality testing. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    for (;; ++k) {
        /* Candidate 18·k + 5 is always odd and ≡ 5 (mod 9). */
        uint64_t cand = 18ULL * k + 5ULL;

        /* Skip primes; count composites. */
        if (!is_prime_small((seq_t)cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
    }
}
