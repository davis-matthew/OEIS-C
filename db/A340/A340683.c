/*
 * A340683  a(n) = A007949((A003961(A003961(n))+1)/2), where A003961 shifts
 *          the prime factorization of n one step towards larger primes, and
 *          A007949(x) gives the exponent of largest power of 3 dividing x.
 * https://oeis.org/A340683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007949((A003961(A003961(n))+1)/2)
 * Range:     u8 n <= 15, u16 n <= 3179, u32 n <= 9221, u64 n <= 9221,
 *            u128 n <= 9221, big any n
 * Helpers:   is_prime_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes OEIS sequence A340683 for n >= 1.
 * Uses the definition:
 *   b(n) = A003961(n) = ∏ next_prime(p)^e  (p^e ∥ n)
 *   c(n) = A003961(b(n)) = ∏ next_prime(next_prime(p))^e
 *   a(n) = v_3( (c(n)+1)/2 )
 * The routine works for all supported seq_t/idx_t types.
 */

#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/seq_mul.h"

static uint64_t next_prime_u64(uint64_t p)
{
    /* Return the smallest prime greater than the given prime p. */
    uint64_t x = p + 1;
    if (x <= 2)
        return 2;
    if ((x & 1ULL) == 0)
        x += 1;                 /* make x odd */
    while (!is_prime_small((seq_t)x))
        x += 2;
    return x;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t tmp   = n;            /* copy of n for factoring          */
    seq_t prod  = 1;            /* product representing c(n)        */

    for (idx_t i = 2; i <= tmp / i; ++i)
    {
        if (tmp % i == 0)
        {
            idx_t exp = 0;
            while (tmp % i == 0)
            {
                tmp /= i;
                ++exp;
            }

            uint64_t p1 = next_prime_u64((uint64_t)i);
            uint64_t p2 = next_prime_u64(p1);

            seq_t base = (seq_t)p2;
            seq_t term = 1;
            for (idx_t j = 0; j < exp; ++j)
                term = seq_mul(term, base);

            prod = seq_mul(prod, term);
        }
    }

    if (tmp > 1)                /* remaining prime factor             */
    {
        uint64_t p1 = next_prime_u64((uint64_t)tmp);
        uint64_t p2 = next_prime_u64(p1);
        prod = seq_mul(prod, (seq_t)p2);   /* exponent is 1 */
    }

    seq_t val = prod + 1;        /* (c(n)+1)/2                         */
    val = val / 2;

    seq_t exp3 = 0;
    while (val % 3 == 0)
    {
        val = val / 3;
        ++exp3;
    }

    return exp3;
}
