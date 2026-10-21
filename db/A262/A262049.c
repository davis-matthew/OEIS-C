/*
 * A262049  Sum of the palindromic primes dividing n (with repetition).
 * https://oeis.org/A262049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the sum of the palindromic primes (with multiplicity)
 *            dividing n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* Trial division up to sqrt(m).  The condition p <= m/p avoids overflow. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (is_palindrome((seq_t)p, (idx_t)10)) {
                sum += (seq_t)e * (seq_t)p;
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1 && is_palindrome((seq_t)m, (idx_t)10)) {
        sum += (seq_t)m;
    }

    return sum;
}
