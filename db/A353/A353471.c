/*
 * A353471  a(n) = 1 if n is a prime or a squarefree semiprime, otherwise 0.
 * https://oeis.org/A353471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=1 if tau(n)=2*omega(n) (i.e. n is a prime or a product of
 *            two distinct primes), else 0
 * Range:     u8 n <= 1409, u16 n <= 9965, u32 n <= 9965, u64 n <= 9965,
 *            u128 n <= 9965, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* 1 is not counted */

    /* Prime test */
    if (is_prime_small((seq_t)n))
        return 1;

    /* Composite number: test for squarefree semiprime */
    uint64_t p = _smallest_prime_factor(n);
    if (p == 0)                      /* should not occur */
        return 0;

    uint64_t q = n / p;
    if (q == p)                      /* square of a prime */
        return 0;

    /* q must be prime and distinct from p */
    return is_prime_small((seq_t)q) ? 1 : 0;
}
