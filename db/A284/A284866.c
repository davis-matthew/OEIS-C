/*
 * A284866  a(n) = prime(n)^prime(n+1) * prime(n+1)^prime(n).
 * https://oeis.org/A284866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^prime(n+1) * prime(n+1)^prime(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* External linkage - function as required */
seq_t a(idx_t n)
{
    /* OEIS offset is 1; undefined for n < 1 – return 0 as a safe default */
    if (n < 1)
        return (seq_t)0;

    /* Retrieve the n‑th and (n+1)-st primes */
    seq_t p = (seq_t)get_prime(n);
    seq_t q = (seq_t)get_prime(n + 1);

    /* Compute the two mixed powers */
    seq_t part1 = big_pow(p, q);
    seq_t part2 = big_pow(q, p);

    /* Multiply the results to get a(n) */
    return seq_mul(part1, part2);
}
