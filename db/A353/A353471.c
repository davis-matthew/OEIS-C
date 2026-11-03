/*
 * A353471  a(n) = 1 if n is a prime or a squarefree semiprime, otherwise 0.
 * https://oeis.org/A353471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=1 if tau(n)=2*omega(n) (i.e. n is a prime or a product of
 *            two distinct primes), else 0
 * Range:     u8 n <= 1409, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9965 (2026-11-01)
 * Helpers:   is_prime_seq, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* 1 is not counted */

    /* Prime test */
    if (is_prime_seq((seq_t)n))
        return 1;

    /* Composite number: test for squarefree semiprime */
    uint64_t p = smallest_prime_factor(n);
    if (p == 0)                      /* should not occur */
        return 0;

    uint64_t q = n / p;
    if (q == p)                      /* square of a prime */
        return 0;

    /* q must be prime and distinct from p */
    return is_prime_seq((seq_t)q) ? 1 : 0;
}
