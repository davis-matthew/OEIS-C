/*
 * A381033  a(n) = 1 if there is a digit > 1 in the primorial base expansion
 *          of n, and the corresponding radix prime of the least significant
 *          such digit is not a prime factor of n, otherwise 0.
 * https://oeis.org/A381033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there is a digit > 1 in the primorial base expansion
 *            of n whose least significant such digit has radix prime dividing
 *            n; otherwise 0
 * Range:     u8 n <= 9904, u16 n <= 9904, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint32_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229
};
static const size_t NUM_PRIMES = sizeof(primes) / sizeof(primes[0]);

seq_t a(idx_t n)
{
    /* Find the least significant digit > 1 in the primorial base
       representation of n and the corresponding radix prime. */

    uint32_t least_sig_prime = 0;
    int found = 0;

    /* weight is the product of primes[0..i-1] (initially 1 for i=0). */
    idx_t weight = 1;

    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        uint32_t p = primes[i];

        if (weight > n) {
            /* Remaining digits will be zero. */
            break;
        }

        /* Compute the digit at this position.
           Since the base at this place is p, the digit is
           (n / weight) % p. */
        idx_t q = n / weight;
        uint32_t d = (uint32_t)(q % p);

        if (!found && d > 1) {
            found = 1;
            least_sig_prime = p;
        }

        /* Update weight for next iteration, guarding against overflow. */
        if (weight <= n / (idx_t)p) {
            weight *= (idx_t)p;
        } else {
            /* Any further weight will exceed n; exit loop. */
            weight = n + 1;
        }
    }

    /* Determine the output value. */
    seq_t result = 0;
    if (found && (n % (idx_t)least_sig_prime) == 0) {
        result = 1;
    }
    return result;
}
