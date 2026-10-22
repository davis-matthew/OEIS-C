/*
 * A361268  Multiplicative with a(p^e) = e * p^(e + 2), e > 0.
 * https://oeis.org/A361268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e||n} e · p^(e+2) (multiplicative function)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* a(p^e) = e * p^(e+2)  for e > 0,  a(1) = 1 */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1; the function is undefined for n=0 */
        return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* factor n */

    /* trial division up to √m */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* compute p^(exp+2) */
        seq_t power = 1;
        for (uint64_t i = 0; i < (exp + 2); ++i)
            power *= (seq_t)p;

        /* term = exp * p^(exp+2) */
        result *= (seq_t)exp * power;
    }

    /* if remaining m > 1, it is a prime factor with exponent 1 */
    if (m > 1) {
        seq_t power = 1;
        for (uint64_t i = 0; i < 3; ++i)          /* exp=1 -> 1+2 = 3 */
            power *= (seq_t)m;
        result *= 1 * power;                     /* exp=1 */
    }

    return result;
}
