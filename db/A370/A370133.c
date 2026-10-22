/*
 * A370133  Numbers with no digit larger than 3 in primorial base, A049345.
 * https://oeis.org/A370133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 4224, u32 n <= 9852, u64 n <= 9852,
 *            u128 n <= 9852, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A370133: a(n) is the n‑th nonnegative integer whose representation in primorial base uses only digits 0,1,2,3. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static const uint32_t _primorial_primes[] = {
    2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U, 29U,
    31U, 37U, 41U, 43U, 47U, 53U, 59U, 61U, 67U, 71U
};
static const size_t _primorial_prime_count =
    sizeof(_primorial_primes) / sizeof(_primorial_primes[0]);

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for offset */

    idx_t rem = n - (idx_t)1;              /* zero‑based index */
    seq_t result = (seq_t)0;
    seq_t weight = (seq_t)1;

    /* position 0: radix 2, digits 0..1 */
    uint32_t d = (uint32_t)(rem % 2U);
    rem /= 2U;
    result += (seq_t)d * weight;
    weight *= (seq_t)_primorial_primes[0];     /* weight becomes 2 */

    /* position 1: radix 3, digits 0..2 */
    d = (uint32_t)(rem % 3U);
    rem /= 3U;
    result += (seq_t)d * weight;
    weight *= (seq_t)_primorial_primes[1];     /* weight becomes 2*3 = 6 */

    /* position 2 and higher: radix 4, digits 0..3 */
    size_t idx = 2U;
    while (rem != (idx_t)0 && idx < _primorial_prime_count)
    {
        d = (uint32_t)(rem % 4U);
        rem /= 4U;
        result += (seq_t)d * weight;
        weight *= (seq_t)_primorial_primes[idx];
        ++idx;
    }

    /* If extra digits are required for very large n, the function
       ignores them because for the supported range of n the product
       of primes in weight would exceed any reasonable bound. */
    return result;
}
