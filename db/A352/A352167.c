/*
 * A352167  a(n) is the sum of the prime factors of n (with multiplicity) that
 *          are less than n.
 * https://oeis.org/A352167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is prime; otherwise sum of prime factors of n with
 *            multiplicity. This implements a(n) = A001414(n) - A061397(n),
 *            where A061397(n) = n if n is prime, else 0
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   is_prime, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases first */
    if (n <= 1) {
        return 0;
    }
    /* If n is prime, by definition a(n) = 0 */
    if (is_prime(n)) {
        return 0;
    }

    /* For composite n, sum the prime factors with multiplicity. */
    seq_t sum = 0;
    uint64_t m = (uint64_t)n;
    while (m > 1) {
        uint64_t p = lpf(m);          /* least prime factor of m */
        sum += (seq_t)p;
        m /= p;
    }
    return sum;
}
