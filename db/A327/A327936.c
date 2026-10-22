/*
 * A327936  Multiplicative with a(p^e) = p if e >= p, otherwise 1.
 * https://oeis.org/A327936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of all primes p dividing n such that the exponent
 *            e of p in the factorisation of n satisfies e >= p
 * Range:     u8 n <= 9224, u16 n <= 9224, u32 n <= 9224, u64 n <= 9224,
 *            u128 n <= 9224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes a( n ) defined above. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1, but return 0 */

    seq_t  prod = 1;       /* result */
    uint64_t x = n;        /* mutable copy */

    /* trial division up to sqrt(x) */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (exp >= p)
                prod *= (seq_t)p;
        }
    }

    /* Any remaining factor is prime > sqrt(original n).
       It appears with exponent 1 < prime, so it is ignored. */
    return prod;
}
