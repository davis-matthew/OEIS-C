/*
 * A249767  a(n) = mu(n) + omega(n).
 * https://oeis.org/A249767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mu(n) + omega(n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* omega(n) = number of distinct prime factors of n */
/* mu(n) is the Möbius function: */
/*   μ(1) = 1; μ(n) = 0 if n is not square‑free; μ(n) = (–1)^k when n is a product of k distinct primes */

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* below offset 1 – undefined for the sequence */
        return 0;

    /* Möbius value as a signed 8‑bit integer */
    int8_t mu = (int8_t)mobius(n);             /* mobius returns seq_t */

    /* ω(n): number of distinct prime factors */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    seq_t result;
    if (mu == -1)            /* primes: μ = –1, ω = 1 → result = 0 */
        result = omega - 1;
    else                     /* μ = 0 or 1 */
        result = omega + (seq_t)mu;

    return result;
}
