/*
 * A332312  Numbers k such that k and k + 1 have the same number of divisors
 *          in Gaussian integers.
 * https://oeis.org/A332312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A332312
    Formula: a(n) = nth integer k such that k and k+1 have the same number of
    divisors in the Gaussian integers.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Return the number of divisors of n in the Gaussian integers.
   d_G(n) = 4 * (2*e_2+1) * ∏_{p≡1 mod4} (e_p+1)^2 * ∏_{q≡3 mod4} (e_q+1)  */
static uint64_t divisor_count_gaussian(uint64_t n)
{
    if (n == 1) return 4;  /* 1 has only the four units as divisors */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    uint64_t res = 4;            /* multiply by the unit group {±1, ±i} */
    uint64_t e2 = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] == 2) { e2 = exps[i]; break; }

    res *= (2 * e2 + 1); /* contribution of the prime 2 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;
        uint64_t e = exps[i];
        if (p % 4 == 1) {           /* split prime */
            uint64_t t = e + 1;
            res *= t * t;            /* (e+1)^2 */
        } else { /* p % 4 == 3 : inert prime */
            res *= (e + 1);
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;     /* array of sequence values */
    static size_t cap   = 0;        /* allocated capacity */
    static size_t used  = 0;        /* number of terms currently stored */

    if (n == 0) return 0; /* offset 1: a(1) is first term */

    size_t need = (size_t)n;
    if (used < need) {
        /* enlarge storage if necessary */
        if (cap < need) {
            cap = need * 2;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* start scanning from the next integer after the last found term */
        uint64_t start = 2;
        if (used > 0) start = (uint64_t)cache[used - 1] + 1;

        uint64_t k = start;
        while (used < need) {
            uint64_t d1 = divisor_count_gaussian(k);
            uint64_t d2 = divisor_count_gaussian(k + 1);
            if (d1 == d2) {
                cache[used++] = (seq_t)k;   /* store term */
            }
            ++k;
        }
    }
    return cache[n - 1];
}
