/*
 * A284327  a(n) is the least positive integer such that n^2 + a(n)^2 and n^2
 *          + (a(n) - 2)^2 are primes.
 * https://oeis.org/A284327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A284327:
   the least positive integer a such that
     n^2 + a^2   is prime   and
     n^2 + (a-2)^2 is prime.
   The function is implemented by a simple brute‑force search. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* seq_t and idx_t are supplied by the build (unsigned integer types). */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; undefined for 0 */
        return (seq_t)0;

    seq_t nsq = (seq_t)n * (seq_t)n;      /* n^2 */

    /* a = 1 is valid only when n^2 + 1 is prime. */
    if (is_prime_big(nsq + (seq_t)1))
        return (seq_t)1;

    /* Search for the smallest a > 1 that satisfies the two primality tests. */
    for (seq_t a_candidate = (seq_t)2; ; ++a_candidate) {
        seq_t a_sq  = a_candidate * a_candidate;
        seq_t diff  = a_candidate - (seq_t)2;        /* a_candidate ≥ 2 here */
        seq_t diff_sq = diff * diff;

        seq_t p1 = nsq + a_sq;
        seq_t p2 = nsq + diff_sq;

        if (is_prime_big(p1) && is_prime_big(p2))
            return a_candidate;
    }
}
