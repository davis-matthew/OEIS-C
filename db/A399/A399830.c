/*
 * A399830  The number of divisors d of n such that gcud(d, n/d) is a square,
 *          where gcud is the greatest common unitary divisor.
 * https://oeis.org/A399830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = multiplicative with a(p^e) = e if e == 2 (mod 4), and e +
 *            1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors d of n such that the greatest common unitary divisor of d and n/d
 * is a square. */
#include <stdint.h>

/* Return the A399830 value for index n (1-based). */
seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* a(1) = 1 */

    uint64_t m = (uint64_t)n;      /* work in 64‑bit integer for factorisation */
    seq_t result = 1;

    /* Factor out powers of 2 */
    uint64_t e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        if ((e & 3ULL) == 2)          /* e mod 4 == 2 */
            result *= (seq_t)e;
        else
            result *= (seq_t)(e + 1);
    }

    /* Factor odd primes up to sqrt(m) */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        e = 0;
        while (m % f == 0) {
            ++e;
            m /= f;
        }
        if (e > 0) {
            if ((e & 3ULL) == 2)
                result *= (seq_t)e;
            else
                result *= (seq_t)(e + 1);
        }
    }

    /* If a prime factor remains > 1, its exponent is 1 (odd) */
    if (m > 1) {
        /* e = 1 => odd => factor e+1 = 2 */
        result *= (seq_t)2;
    }

    return result;
}
