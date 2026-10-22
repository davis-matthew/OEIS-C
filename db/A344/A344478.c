/*
 * A344478  Number of unitary prime divisors p of n such that n/p is
 *          squarefree.
 * https://oeis.org/A344478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω(n) * μ(n)^2 (number of distinct prime factors of n if n
 *            is square‑free, otherwise 0)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   count_distinct_primes, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Count distinct prime factors only when n is square‑free */
    if (is_squarefree((seq_t)n))
        return (seq_t)count_distinct_primes((uint64_t)n);
    else
        return (seq_t)0;
}
