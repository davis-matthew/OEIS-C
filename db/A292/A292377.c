/*
 * A292377  a(1) = 0, and for n > 1, a(n) = a(A252463(n)) + [n == 3 (mod 4)].
 * https://oeis.org/A292377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n>1, a(n) = a(A252463(n)) + [n == 3 (mod 4)]
 * Range:     u8 n <= 2283, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252463(n) divides even n by 2; if n is odd it replaces each prime factor
 * p^e by the previous prime p_prev^e.                                         */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_big.h"

/* Return the largest prime less than p.  p is assumed to be an odd prime > 2. */
static inline seq_t previous_prime(seq_t p)
{
    seq_t cand = p - 1;
    while (cand > (seq_t)2 && !is_prime_big(cand))
        cand -= (seq_t)1;
    return cand;
}

/* A252463: n -> n/2 if n even; otherwise replace each odd prime factor by 
 * the previous prime.  The resulting number is < = n and fits in idx_t. */
static idx_t map_a252463(idx_t n)
{
    if ((n & 1u) == 0u)   /* even */
        return n >> 1;    /* n/2 */

    /* odd: factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64(n, primes, exps, &cnt);

    idx_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prevp = previous_prime((seq_t)primes[i]);   /* previous prime */
        /* multiply prevp ^ exps[i] */
        for (uint64_t j = 0; j < exps[i]; ++j)
            res = (idx_t)((seq_t)res * (seq_t)prevp);
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1u)   /* base case a(1) = 0, and 0 is out of range */
        return 0;

    seq_t add = ((n & 3u) == 3u) ? 1 : 0;
    seq_t part = a(map_a252463(n));
    return part + add;
}
