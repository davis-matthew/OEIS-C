/*
 * A250030  a(n) is the number of iterations of A008474 needed to reach 5,
 *          starting at n, where A008474(Product p_i^e_i) = Sum(p_i+e_i).
 * https://oeis.org/A250030
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of A008474 needed to reach 5, /*
 *            starting from n. Initial values: a(5)=0, a(6)=3, a(7)=2,
 * Range:     u8 n <= 4736, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008474(m) = Σ (prime_i + exponent_i) in the prime factorisation of m */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* gives factor_u64(uint64_t,…) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 5.  For smaller indices we
       return 0 but the value is not used. */
    if (n < 5) return 0;

    seq_t steps = 0;
    seq_t m     = n;

    while (m != (seq_t)5) {
        /* Factor the current number.  m is always <= UINT64_MAX
           for the intended range of n (<= 10 000 or even 2⁶⁴‑1). */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt = 0;

        factor_u64((uint64_t)m, primes, exps, &cnt);

        /* Compute the next value f(m).  All additions stay within seq_t. */
        seq_t next = 0;
        for (size_t i = 0; i < cnt; ++i) {
            next += (seq_t)primes[i] + (seq_t)exps[i];
        }

        m     = next;
        ++steps;
    }

    return steps;
}
