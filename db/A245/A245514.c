/*
 * A245514  Smallest m such that at least one of the two odd numbers which
 *          bracket n^m is not a prime.
 * https://oeis.org/A245514
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m ≥ 1 such that at least one of the two odd
 *            integers that bracket n^m is not prime. For even n the bracketed
 *            pair is (n^m−1, n^m+1); for odd n the pair is (n^m−2, n^m+2)
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n ≥ 2 (offset 2).  Return 1 for n=2,5 etc. */
    uint64_t m = 1;          /* local counter, value fits in native type */
    while (1) {
        /* Compute n^m as a seq_t value */
        seq_t power = seq_pow((seq_t)n, m);

        seq_t left, right;
        if ((n & 1ULL) == 0) {          /* even n → neighbors +/-1 */
            left  = power - 1;
            right = power + 1;
        } else {                        /* odd n → neighbors +/-2 */
            left  = power - 2;
            right = power + 2;
        }

        /* If either neighbor is composite (or < 2), we have found the smallest m */
        if (!is_prime_big(left) || !is_prime_big(right))
            return (seq_t)m;

        ++m;   /* otherwise try the next exponent */
    }
}
