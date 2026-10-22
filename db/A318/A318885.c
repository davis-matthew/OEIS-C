/*
 * A318885  If n = p^a * q^b * ... * r^c, with p < q < r primes, with nonzero
 *          exponents a, b, c, then a(n) = prime(1+p-p)^a * prime(1+q-p)^b *
 *          ... * prime(1+r-p)^c; a(1) = 1.
 * https://oeis.org/A318885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ over primes p of n ( prime(1+p-p₁) )^e_p with p₁ =
 *            smallest prime divisor of n where n = ∏ p^e_p, p₁ < … < p. For
 *            n=1, a(1)=1
 * Range:     u8 n <= 73, u16 n <= 3717, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   big_pow, factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper routines provided by the build environment.                 */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */

/* Compute the A318885 sequence term for index n (n ≥ 1).              */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Allocate buffers large enough to hold the prime factorisation of n. */
    uint64_t primes[64] = {0};
    uint64_t exps[64]   = {0};
    size_t   cnt        = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* The smallest prime factor is primes[0].                             */
    uint64_t p1 = primes[0];
    seq_t    res = big_pow((seq_t)2, (seq_t)exps[0]); /* prime(1)=2 */

    /* Process the remaining prime factors.                               */
    for (size_t i = 1; i < cnt; ++i)
    {
        uint64_t diff = primes[i] - p1;                 /* p - p₁          */
        idx_t    idx  = (idx_t)(1 + diff);              /* index of prime   */
        seq_t    w    = big_pow((seq_t)get_prime(idx), (seq_t)exps[i]);
        res *= w;
    }

    return res;
}
