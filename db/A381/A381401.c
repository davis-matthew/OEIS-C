/*
 * A381401  a(n) is the number of (possibly non-distinct) prime elements in
 *          the multiset of bases and exponents in the prime factorization of
 *          n.
 * https://oeis.org/A381401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime elements in the multiset {p_i, e_i}
 *            derived from the prime factorization n = ∏ p_i^e_i
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t count = 0;
    uint64_t v = (uint64_t)n;
    while (v > 1) {
        uint64_t p = _smallest_prime_factor(v);
        count += 1;                /* the base prime */
        uint64_t e = 0;
        while (v % p == 0) {
            v /= p;
            e++;
        }
        if (is_prime_small((seq_t)e)) count += 1;   /* prime exponent? */
    }
    return count;
}
