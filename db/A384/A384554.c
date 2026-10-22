/*
 * A384554  The sum of the infinitary divisors of n that are cubefree.
 * https://oeis.org/A384554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n=1; otherwise a is multiplicative with a(p^e) = 1
 *            if e≡0 (mod 4) p+1 if e≡1 (mod 4) p²+1 if e≡2 (mod 4) p²+p+1 if
 *            e≡3 (mod 4)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* The OEIS utilities are not required for the implementation.  */
static const uint64_t _SMALL_PRIMES[] =
    {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};

seq_t a(idx_t n)
{
    /* n is at least 1. For n = 1 the product is empty and equals 1. */
    uint64_t x = (uint64_t)n;
    seq_t result = 1;

    /* Trial‑division factorisation; n ≤ 10 000 guarantees at most 25 primes. */
    for (size_t i = 0; i < sizeof(_SMALL_PRIMES)/sizeof(_SMALL_PRIMES[0]); ++i)
    {
        uint64_t p = _SMALL_PRIMES[i];
        if (p * p > x) break;

        if (x % p == 0)
        {
            uint64_t e = 0;
            while (x % p == 0)
            {
                x /= p;
                ++e;
            }

            uint64_t r = e & 3ULL;          /* e mod 4 */
            seq_t term;
            switch (r)
            {
                case 0: term = 1; break;
                case 1: term = (seq_t)p + 1; break;
                case 2: term = (seq_t)p * (seq_t)p + 1; break;
                case 3: term = (seq_t)p * (seq_t)p + (seq_t)p + 1; break;
                default: term = 1; /* unreachable */
            }
            result *= term;
        }
    }

    /* Remaining factor, if any, is a prime > 100. */
    if (x > 1)
    {
        /* exponent is 1, so r = 1 */
        result *= (seq_t)x + 1;
    }

    return result;
}
