/*
 * A292285  GCD of orders (in GF(p)) of primes dividing p-1, for odd primes p.
 * https://oeis.org/A292285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = GCD of orders (in GF(p)) of primes dividing p-1, for odd
 *            primes p
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, mod_pow, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Obtain the nth prime (1‑based).  For n=2 this gives p=3. */
    uint32_t p = get_prime(n);
    uint64_t p_minus1 = (uint64_t)p - 1ULL;

    /* Factor p-1.  primes[0..cnt-1] contain the distinct prime factors. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(p_minus1, primes, exps, &cnt);

    seq_t gcd_val = 0;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t q = (seq_t)primes[i];                   /* the base for the order */
        seq_t ord = (seq_t)p_minus1;                  /* start with p-1 */

        /* Reduce ord to the multiplicative order of q mod p. */
        for (size_t j = 0; j < cnt; ++j)
        {
            seq_t r = (seq_t)primes[j];
            while (ord % r == 0)
            {
                seq_t candidate = ord / r;
                if (mod_pow(q, (uint64_t)candidate, (seq_t)p) == 1)
                    ord = candidate;
                else
                    break;
            }
        }

        if (gcd_val == 0)
            gcd_val = ord;
        else
            gcd_val = seq_gcd(gcd_val, ord);
    }

    return gcd_val;
}
