/*
 * A355684  Dirichlet inverse of A355448.
 * https://oeis.org/A355684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1)=1. For a prime power p^e: a(p)=0, and if e≥2 a(p^e) = -1
 *            when e ≡ 2 or 3 (mod 4), otherwise a(p^e) = 1. The sequence is
 *            multiplicative: a(m n)=a(m)·a(n) for gcd(m,n)=1
 * Range:     i8 n <= 8133, i16 n <= 8133, i32 n <= 8133, i64 n <= 8133,
 *            i128 n <= 8133, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External linkage; returns the sequence value for index n (n≥1). */
seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1)=1, also safe if someone passes 0 */
        return 1L;

    seq_t result = 1L;
    uint64_t k = (uint64_t)n;

    /* Factorise k by trial division. */
    for (uint64_t p = 2; p * p <= k; ++p)
    {
        if (k % p != 0)
            continue;

        /* Count exponent of current prime p. */
        unsigned e = 0;
        while (k % p == 0)
        {
            k /= p;
            ++e;
        }

        if (e == 1)          /* a(p)=0 → whole product is zero */
            return 0L;

        /* For e≥2: flip sign when e mod 4 is 2 or 3 */
        if ((e & 3) == 2 || (e & 3) == 3)
            result = -result;
    }

    /* If remaining factor k>1, it is a prime with exponent 1. */
    if (k > 1)
        return 0L;

    return result;
}
