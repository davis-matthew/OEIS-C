/*
 * A241534  Number of integer arithmetic means of 2 distinct divisors of n.
 * https://oeis.org/A241534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(oddDiv(n),2) + C(evenDiv(n),2) where oddDiv(n) = number
 *            of odd divisors of n evenDiv(n) = number of even divisors of n
 *            This equals the number of unordered pairs of distinct divisors
 *            of n whose arithmetic mean is an integer
 * Range:     u8 n <= 719, u16 n <= 9981, u32 n <= 9981, u64 n <= 9981,
 *            u128 n <= 9981, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Type aliases supplied by the build system */
extern struct { } *temp_not_used;  // silence unused warnings
/* seq_t and idx_t are defined elsewhere by the build system. */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 (offset 1). */
    uint64_t val = (uint64_t)n;

    /* Remove powers of 2 first.  These contribute only to even divisors. */
    int exp2 = 0;
    while ((val & 1ULL) == 0ULL) {   /* faster to test lowest bit */
        exp2++;
        val >>= 1;
    }

    /* divisor counts */
    seq_t tau_n = 1;   /* total number of divisors of n   */
    seq_t tau_m = 1;   /* number of odd divisors (without 2) */

    /* factor the remaining odd part of n */
    uint64_t p = 3;
    while (p <= val / p) {
        if ((val % p) == 0) {
            int exp = 0;
            while ((val % p) == 0) {
                exp++;
                val /= p;
            }
            seq_t mult = (seq_t)exp + 1;
            tau_n *= mult;
            tau_m *= mult;
        }
        p += 2;
    }

    /* If a prime > 1 remains */
    if (val > 1) {
        seq_t mult = 2;           /* exponent 1 -> (1+1) */
        tau_n *= mult;
        tau_m *= mult;
    }

    /* include the factor contributed by 2^exp2 */
    if (exp2 > 0) {
        seq_t mult = (seq_t)exp2 + 1;
        tau_n *= mult;
        /* tau_m is unchanged because odd divisors exclude the 2's */
    }

    seq_t odd  = tau_m;
    seq_t even = tau_n - tau_m;

    seq_t result = 0;
    if (odd > 1)   result += (odd * (odd - 1)) / 2;
    if (even > 1)  result += (even * (even - 1)) / 2;

    return result;
}
