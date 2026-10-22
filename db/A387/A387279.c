/*
 * A387279  Inverse Möbius transform of A006530.
 * https://oeis.org/A387279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A006530(d) (greatest prime divisor of each
 *            divisor, gpf(1)=1) Equivalent to a(n) = 1 + Σ_{i} p_i * e_i *
 *            Π_{j<i} (e_j+1) with primes sorted ascending
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the n-th term of sequence A387279.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety for n==0, not part of OEIS offset */

    seq_t sum = 1;                      /* gpf(1) contributes 1 */

    /* Factorization of n into primes p_i^e_i (ascending order). */
    uint64_t remaining = (uint64_t)n;
    uint64_t primes[64];
    unsigned int exps[64];
    size_t pcnt = 0;

    /* Handle factor 2 separately */
    if ((remaining & 1ULL) == 0) {
        unsigned int e = 0;
        while ((remaining & 1ULL) == 0) {
            remaining >>= 1;
            ++e;
        }
        primes[pcnt] = 2;
        exps[pcnt] = e;
        ++pcnt;
    }

    /* Trial division by odd numbers up to sqrt(remaining) */
    for (uint64_t p = 3; p <= remaining; p += 2) {
        if (remaining % p == 0) {
            unsigned int e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }

    /* Any leftover >1 is a prime factor with exponent 1 */
    if (remaining > 1) {
        primes[pcnt] = remaining;
        exps[pcnt] = 1;
        ++pcnt;
    }

    /* Compute the sum using the compact formula */
    seq_t pref = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        sum += (seq_t)primes[i] * (seq_t)exps[i] * pref;
        pref *= (seq_t)(exps[i] + 1);
    }

    return sum;
}
