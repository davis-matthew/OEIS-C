/*
 * A329615  Bitwise-AND of exponents of prime factors of A108951(n), where
 *          A108951 is fully multiplicative with a(prime(i)) = prime(i)# =
 *          Product_{i=1..i} A000040(i).
 * https://oeis.org/A329615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise AND of exponents of prime factors of A108951(n) =
 *            bitwise AND of suffix sums of the exponents of the prime
 *            factorization of n, where the suffix sum for a prime p is the
 *            sum of the exponents of all primes in n that are >= p
 * Range:     u8 n <= 9947, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   factorize_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize_simple.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 : no prime factors in A108951(1)=1 */
    if (n <= 1u)
        return 0;

    /* factor n */
    uint64_t primes[128];
    int       exp[128];
    size_t    cnt = factorize_simple(n, primes, exp);
    if (cnt == 0)            /* safety: should not happen for n>1 */
        return 0;

    /* sort by ascending prime so that suffix sums work correctly */
    for (size_t i = 1; i < cnt; ++i) {
        size_t j = i;
        while (j > 0 && primes[j - 1] > primes[j]) {
            /* swap primes */
            uint64_t t0 = primes[j - 1];
            primes[j - 1] = primes[j];
            primes[j] = t0;
            /* swap exponents */
            int t1 = exp[j - 1];
            exp[j - 1] = exp[j];
            exp[j] = t1;
            --j;
        }
    }

    /* compute suffix sums of exponents */
    seq_t suffix[128];
    seq_t sum = 0;
    for (size_t i = cnt; i-- > 0;) {
        sum += (seq_t)exp[i];
        suffix[i] = sum;
    }

    /* bitwise AND of all suffix sums */
    seq_t res = ~ (seq_t)0;   /* all bits set */
    for (size_t i = 0; i < cnt; ++i)
        res &= suffix[i];

    return res;
}
