/*
 * A384421  The number of exponentially squarefree prime powers (not including
 *          1) that unitarily divide n.
 * https://oeis.org/A384421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime power divisors p^e of n with e
 *            square‑free (i.e. e has no repeated prime factor). For n=1 the
 *            value is 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether an integer e is squarefree.  e is guaranteed to be
   small (at most log₂(n)), so a simple loop suffices. */
static bool is_squarefree(idx_t e)
{
    if (e <= 1) return true; /* 1 is considered squarefree */
    for (idx_t i = 2; i * i <= e; ++i) {
        if (e % (i * i) == 0) return false;
    }
    return true;
}

/* External link function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    seq_t result = 0;
    uint64_t m = (uint64_t)n;

    /* Trial division up to sqrt(m).  The loop guard uses division
       to avoid overflow when p*p may exceed uint64_t. */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            if (is_squarefree(exp)) {
                ++result;
            }
        }
    }

    /* If m > 1, it is a prime factor with exponent 1. */
    if (m > 1 && is_squarefree(1)) {
        ++result;
    }

    return result;
}
