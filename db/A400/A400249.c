/*
 * A400249  Möbius transform of A085731, where A085731(n) = gcd(n,n'), and n'
 *          is the arithmetic derivative of n.
 * https://oeis.org/A400249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(1)=1 and a is multiplicative with a(p^e) = p^e - p^(e-2) if e
 *            ≡ 0 (mod p) 0 if e-1 ≡ 0 (mod p) p^(e-1) - p^(e-2) otherwise (p
 *            prime, e≥1)
 * Range:     u8 n <= 431, u16 n <= 9257, u32 n <= 9257, u64 n <= 9257,
 *            u128 n <= 9257, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

static seq_t a_prime_power(idx_t p, idx_t e)
{
    if ((e % p) == 0)                 /* e multiple of p (e ≥ p ≥ 2) */
    {
        seq_t p_pow_e = pow_seq((seq_t)p, e);
        seq_t p_pow_e_minus2 = pow_seq((seq_t)p, e - 2);
        return p_pow_e - p_pow_e_minus2;
    }
    else if (((e - 1) % p) == 0)     /* e-1 multiple of p */
    {
        return 0;
    }
    else                             /* ordinary case */
    {
        seq_t p_pow_e_minus1 = pow_seq((seq_t)p, e - 1);
        seq_t p_pow_e_minus2 = pow_seq((seq_t)p, e - 2);
        return p_pow_e_minus1 - p_pow_e_minus2;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    uint64_t n64 = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t p = (idx_t)primes[i];
        idx_t e = (idx_t)exps[i];
        result *= a_prime_power(p, e);
    }
    return result;
}
