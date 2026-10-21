/*
 * A293482  The number of 5th powers in the multiplicative group modulo n.
 * https://oeis.org/A293482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of 5th power residues in the multiplicative group
 *            modulo n. Computation uses multiplicativity: a(n) = Π_p a(p^e)
 *            where a(p^e) is given by the formulas listed in the OEIS entry
 * Range:     u8 n <= 256, u16 n <= 7548, u32 n <= 7548, u64 n <= 7548,
 *            u128 n <= 7548, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* Externally provided prime factorisation */

/* Small binary exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Compute a(p^e) for a prime power, according to the OEIS formulas. */
static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    if (p == 2)
    {
        /* a(2^e) = 2^(e-1) for all e ≥ 1  (a(1)=1 when e=0). */
        return pow_seq((seq_t)2, e ? e - 1 : 0);
    }
    else if (p == 5)
    {
        if (e == 1) return 4;
        seq_t mult = pow_seq((seq_t)5, e - 2);   /* 5^(e-2) */
        return (seq_t)4 * mult;                  /* 4 * 5^(e-2) */
    }
    else
    {
        /* For primes p ≠ 2,5:
           a(p^e) = (p-1)*p^(e-1)        if p ≡ 2,3,4 (mod 5)
           a(p^e) = (p-1)*p^(e-1)/5      if p ≡ 1 (mod 5) */
        seq_t term = pow_seq((seq_t)p, e - 1);
        term = term * ((seq_t)p - 1);
        if (p % 5 == 1)
            term = term / 5;
        return term;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined by OEIS, safe fallback */
    if (n == 1) return 1;    /* a(1) = 1 */

    /* factor_u64 expects pre-allocated arrays; 64 is ample for n ≤ 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= a_prime_power(primes[i], exps[i]);

    return result;
}
