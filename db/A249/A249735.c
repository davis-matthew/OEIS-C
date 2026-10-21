/*
 * A249735  Odd bisection of A003961: Replace in 2n-1 each prime factor p(k)
 *          with prime p(k+1).
 * https://oeis.org/A249735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e | (2n-1)} next_prime(p)^e
 * Range:     u8 n <= 31, u16 n <= 1093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where the product runs over the prime factorisation of the odd number 2n-1 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n,uint64_t *pr,uint64_t *ex,size_t *cnt) */
#include "../utils/next_prime.h"   /* next_prime(uint64_t start) returns the next prime after start */

/* Multiply a base to a power e (e >= 0), result stored in seq_t */
static seq_t pow_seq(seq_t base, unsigned int e)
{
    seq_t result = 1;
    while (e)
    {
        if (e & 1U)
            result *= base;
        base *= base;
        e >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset 1, but for safety */

    /* 2n-1 fits in uint64_t for n up to (UINT64_MAX+1)/2 */
    uint64_t m = (uint64_t)((unsigned __int128)n * 2u - 1u);

    /* factorise m */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute the product of next primes to the corresponding exponents */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p_next = next_prime(primes[i]);   /* next prime after the original prime factor */
        result *= pow_seq((seq_t)p_next, (unsigned int)exps[i]);
    }
    return result;
}
