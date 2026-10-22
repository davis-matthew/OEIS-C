/*
 * A377519  The number of divisors of n that are terms in A207481.
 * https://oeis.org/A377519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of (min(p,e)+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Working copy of n (as 64‑bit unsigned) */
    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Trial division up to sqrt(nn) using division to avoid overflow */
    for (uint64_t p = 2; p <= nn / p; ++p) {
        if (nn % p == 0) {
            uint64_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            /* Compute min(p, exp) in 64‑bit, then cast to seq_t */
            uint64_t min_pe = (p < exp) ? p : exp;
            result *= (seq_t)(min_pe + 1);
        }
    }

    /* If nn > 1, it is a prime factor with exponent 1 */
    if (nn > 1) {
        /* For a prime exponent 1, min(p,1) = 1, so factor = 2 */
        result *= (seq_t)2;
    }

    return result;
}
