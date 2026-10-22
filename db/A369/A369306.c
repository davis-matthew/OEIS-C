/*
 * A369306  The number of cubefree divisors d of n such that n/d is also
 *          cubefree.
 * https://oeis.org/A369306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(e) where * f(1)=2, f(2)=3, f(3)=2, f(4)=1,
 *            f(e)=0 for e≥5 * (i.e. the number of cubefree divisors d of n
 *            such that n/d is cubefree)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static bool _primes_inited = false;
static uint64_t *_primes = NULL;
static size_t _primes_cnt = 0;

/* initialise a static list of small primes up to 100000 */
static void _init_primes(void)
{
    if (!_primes_inited)
    {
        size_t cnt;
        /* Using 100000 as a safe upper bound for trial‑division in this function. */
        build_primes_upto(100000, &_primes, &cnt);
        _primes_cnt = cnt;
        _primes_inited = true;
    }
}

seq_t a(idx_t n)
{
    /* Special case n = 1 (product over empty set) */
    if (n == 1)
        return (seq_t)1;

    _init_primes();

    uint64_t x = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trial division by small primes */
    for (size_t i = 0; i < _primes_cnt && _primes[i] * _primes[i] <= x; ++i)
    {
        uint64_t p = _primes[i];
        if (x % p == 0)
        {
            unsigned int exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
                if (exp >= 5)        /* f(e)=0 for e≥5 → overall product 0 */
                    return (seq_t)0;
            }
            /* compute multiplicative factor for this prime power */
            seq_t mult;
            switch (exp)
            {
                case 1: mult = (seq_t)2; break;
                case 2: mult = (seq_t)3; break;
                case 3: mult = (seq_t)2; break;
                case 4: mult = (seq_t)1; break;
                default: mult = (seq_t)0; /* unreachable because exp<5 */
            }
            result *= mult;
        }
    }

    /* leftovers are a prime factor > sqrt(original n) */
    if (x > 1)
    {
        /* exponent is 1 */
        result *= (seq_t)2;
    }

    return result;
}
