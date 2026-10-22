/*
 * A359305  Number of divisors of 6*n-1 of form 6*k+1.
 * https://oeis.org/A359305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(A016969(n-1))/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (Number of divisors of 6*n-1 congruent to 1 (mod 6)) */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against accidental call */

    /* 6*n-1 fits in uint64_t for typical n (and certainly for n≤10⁴) */
    uint64_t m = 6 * (uint64_t)n - 1;

    /* factor m   (pr[], ex[] hold primes and exponents) */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t total = 1;   /* product of (e+1) over all prime factors   */
    seq_t alt   = 1;   /* product of (ev - od) over primes (mod6==5)
                         multiplied by (e+1) for primes ≡1 (mod6) */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p  = pr[i];
        uint64_t exp = ex[i];
        seq_t plen = (seq_t)(exp + 1);
        total *= plen;

        if (p % 6 == 5)
        {
            /* For primes ≡5 (mod 6) the parity of the exponent matters
               ev = number of even exponents, od = number of odd exponents */
            seq_t ev = (seq_t)(exp / 2 + 1);          /* even exponents */
            seq_t od = plen - ev;                    /* odd exponents  */
            seq_t diff = ev - od;                    /* either 1 or 0 */
            alt *= diff;
        }
        else /* p % 6 == 1 */
        {
            /* These primes never affect the parity of the residue */
            alt *= plen;
        }
    }

    /* Count of residue-1 divisors = (total + alt)/2   */
    return (total + alt) / 2;
}
