/*
 * A348048  a(n) = sigma(n) / gcd(sigma(n), A003959(n)), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A348048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) / gcd( sigma(n), A003959(n) )
 * Range:     u8 n <= 22, u16 n <= 7321, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided factor_u64 and gcd utilities. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety, though sequence is defined for n>=1 */

    /* Factor n using the provided helper */
    uint64_t pr[128];
    uint64_t ex[128];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sigma = 1;            /* product of sigma(p^e) */
    seq_t a003959 = 1;          /* product of (p+1)^e */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)pr[i];
        idx_t  e = (idx_t)ex[i];

        /* sigma(p^e) = (p^(e+1)-1)/(p-1) */
        seq_t power = 1;
        for (idx_t j = 0; j < e + 1; ++j) power *= p;
        seq_t sigma_comp = (power - 1) / (p - 1);
        sigma *= sigma_comp;

        /* (p+1)^e for A003959(n) */
        seq_t power_plus1 = 1;
        seq_t p_plus = p + 1;
        for (idx_t j = 0; j < e; ++j) power_plus1 *= p_plus;
        a003959 *= power_plus1;
    }

    seq_t g = gcd(sigma, a003959);
    return sigma / g;
}
