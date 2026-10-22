/*
 * A373477  a(n) = 1 if A001414(n) and A003415(n) are both multiples of 3,
 *          otherwise 0, where A001414 is the sum of prime factors with
 *          repetition, and A003415 is the arithmetic derivative.
 * https://oeis.org/A373477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A001414(n) and A003415(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9513, u16 n <= 9513, u32 n <= 9513, u64 n <= 9513,
 *            u128 n <= 9513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes whether n satisfies the divisibility condition.  The calculation
   is performed using only integer arithmetic; no auxiliary libraries are
   required.  The function works for all n >= 1, independent of the
   order of calls. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n starts from 1; a(0) is not defined but returned as 0 for safety. */
    if (n == 0) return (seq_t)0;

    uint64_t orig = (uint64_t)n;     /* original value for derivative */
    uint64_t tmp  = orig;            /* temporary copy for factorisation */

    int sum_mod3 = 0;                /* A001414(n) mod 3 */
    int der_mod3 = 0;                /* A003415(n) mod 3 */

    /* Trial division up to sqrt(tmp).  The bound uses tmp/p to avoid
       overflow. */
    for (uint64_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            /* Contribution of the prime factor to the sum of prime
               factors (with repetition). */
            sum_mod3 = (sum_mod3 + (int)((p % 3) * (e % 3))) % 3;

            /* Contribution to the arithmetic derivative:
               e * (n / p).  Only the value modulo 3 is required. */
            uint64_t part = orig / p;
            int term = (int)(part % 3);
            int add  = (int)(((e % 3) * term) % 3);
            der_mod3 = (der_mod3 + add) % 3;
        }
    }

    /* Remaining prime factor greater than sqrt(orig), if any. */
    if (tmp > 1)
    {
        uint64_t p = tmp;   /* prime factor */
        int e = 1;          /* exponent is always 1 here */

        sum_mod3 = (sum_mod3 + (int)(p % 3)) % 3;

        uint64_t part = orig / p;
        int term = (int)(part % 3);
        int add  = term;   /* e == 1 */
        der_mod3 = (der_mod3 + add) % 3;
    }

    return ((sum_mod3 == 0 && der_mod3 == 0) ? (seq_t)1 : (seq_t)0);
}
