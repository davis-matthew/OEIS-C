/*
 * A370077  The product of exponents of the prime factorization of the largest
 *          unitary divisor of n that is a term of A138302.
 * https://oeis.org/A370077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of the exponents e of the prime factorization of
 *            n for which e is a power of two. For n = 1 the result is 1. The
 *            sequence is multiplicative with a(p^e) = e if e is a power of
 *            two, otherwise a(p^e) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline bool is_power_of_two_uint64(uint64_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;                /* outside the offset; return 1 as for a(1) */

    /* Work on a 64‑bit copy of n for factorisation */
    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 separately */
    uint64_t exp = 0;
    while ((nn & 1) == 0) {
        exp++;
        nn >>= 1;
    }
    if (exp > 0 && is_power_of_two_uint64((uint64_t)exp))
        result *= (seq_t)exp;

    /* factor odd primes */
    uint64_t divisor = 3;
    while (divisor <= nn / divisor) {
        exp = 0;
        while (nn % divisor == 0) {
            exp++;
            nn /= divisor;
        }
        if (exp > 0 && is_power_of_two_uint64((uint64_t)exp))
            result *= (seq_t)exp;

        divisor += 2;     /* skip even numbers */
    }

    /* if something is left, it is a prime factor with exponent 1 */
    if (nn > 1) {
        /* exponent is 1, which is a power of two, so it does not change result */
        /* but for completeness we could multiply by 1 */
    }

    return result;
}
