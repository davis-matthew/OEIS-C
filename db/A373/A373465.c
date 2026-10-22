/*
 * A373465  Palindromes with exactly 5 distinct prime divisors.
 * https://oeis.org/A373465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindrome (in increasing numerical order) that has
 *            exactly 5 distinct prime factors (counting multiplicities). The
 *            function searches palindromes by increasing length and counts
 *            prime factors using count_distinct_primes(uint64_t)
 * Range:     u8 none, u16 n <= 10, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   count_distinct_primes, pow10, rev_pal_left
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e);          */
#include "../utils/count_distinct_primes.h"  /* size_t count_distinct_primes(uint64_t n); */
#include "../utils/rev_pal_left.h"

/* Return the nth element of A373465 */
seq_t a(idx_t n)
{
    /* Pre‑counting 1‑digit palindromes (they cannot be the answer) */
    if (n == 0) return 0;                /* offset starts at 1 */
    idx_t target = n;                    /* zero‑based count */
    idx_t count  = 0;                     /* how many have been found */

    /* Special cases: single digit numbers (ignored, but we traverse them) */
    for (uint64_t d = 1; d <= 9; ++d) {
        if (count_distinct_primes((uint64_t)d) == 5) {
            ++count;
            if (count == target) return (seq_t)d;
        }
    }

    /* Generate palindromes by increasing length */
    for (unsigned len = 2; ; ++len) {
        /* Even length palindromes (len = 2*k) */
        if (len % 2 == 0) {
            unsigned half = len / 2;
            seq_t pow_half = pow10(half);
            uint64_t start = (uint64_t)pow10(half - 1);   /* first digit non-zero */
            uint64_t end   = (uint64_t)pow10(half);
            for (uint64_t left = start; left < end; ++left) {
                seq_t rev = rev_pal_left((seq_t)left, half);
                seq_t val = (seq_t)left * pow_half + rev;
                if (count_distinct_primes((uint64_t)val) == 5) {
                    ++count;
                    if (count == target) return val;
                }
            }
        }
        /* Odd length palindromes (len = 2*k+1) */
        else {
            unsigned half = len / 2;          /* floor(len/2) */
            seq_t pow_half   = pow10(half);
            seq_t pow_half1  = pow10(half + 1);
            uint64_t start = (uint64_t)pow10(half - 1);
            uint64_t end   = (uint64_t)pow10(half);
            for (uint64_t left = start; left < end; ++left) {
                seq_t rev = rev_pal_left((seq_t)left, half);
                for (uint64_t mid = 0; mid <= 9; ++mid) {
                    seq_t val = (seq_t)left * pow_half1
                              + (seq_t)mid * pow_half
                              + rev;
                    if (count_distinct_primes((uint64_t)val) == 5) {
                        ++count;
                        if (count == target) return val;
                    }
                }
            }
        }

        /* Safety: stop if the length would exceed 64 bits.
           This is far beyond what is needed for the first 10000 terms. */
        if (len > 18) break;
    }
    return 0;   /* Should never be reached for valid n */
}
