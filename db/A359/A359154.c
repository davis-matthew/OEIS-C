/*
 * A359154  a(n) = (-1)^sopfr(n), where sopfr is the sum of prime factors with
 *          repetition.
 * https://oeis.org/A359154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (-1)^{sopfr(n)} where sopfr(n) is the sum of * prime
 *            factors of n counted with multiplicity. * Since each odd prime
 *            contributes (exponent mod 2) to the parity of sopfr, * a(n) = +1
 *            if the number of odd prime factors having odd exponent is even,
 *            * and -1 otherwise
 * Range:     i8 n <= 9110, i16 n <= 9110, i32 n <= 9110, i64 n <= 9110,
 *            i128 n <= 9110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, uint32_t */
#include <stdbool.h>  /* for bool */

/* external linkage as required */
seq_t a(idx_t n)
{
    /* n is given with offset 1; we accept n >= 1 */
    uint64_t m = (uint64_t)n;          /* convert index to a working value */
    int parity = 0;                    /* 0 → even sopfr, 1 → odd sopfr */

    /* remove all factors of 2 – they never change parity (2*k is even) */
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;                       /* divide by 2 */
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        int even = 0;                  /* count exponent mod 2 */
        while (m % p == 0ULL) {
            even ^= 1;                 /* toggle on each division */
            m /= p;
        }
        if (even)                      /* p has odd exponent */
            parity ^= 1;
    }

    /* if a prime factor > sqrt remains, its exponent is 1 (odd) */
    if (m > 1ULL) {
        parity ^= 1;
    }

    return (seq_t)(parity ? -1 : 1);
}
