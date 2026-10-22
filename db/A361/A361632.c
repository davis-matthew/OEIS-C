/*
 * A361632  a(n) is the numerator of the median of the prime factors of n with
 *          repetition.
 * https://oeis.org/A361632
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = the numerator of the median of the prime factorisation of
 *            n * (with multiplicity). For an odd number of prime factors the
 *            median is a * single prime; for an even number the median is the
 *            arithmetic mean of the * two middle primes. The numerator is the
 *            integer part of this mean or, * if the...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Only n >= 2 are defined in the OEIS sequence; return 0 for
     * n < 2 to keep the function total. */
    if (n < 2)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;
    /* Worst case: n = 2^63 → 63 factors.  An array of 128 is more than enough. */
    uint64_t factors[128];
    size_t cnt = 0;

    /* Extract factors of 2. */
    while ((m & 1ULL) == 0) {
        factors[cnt++] = 2ULL;
        m >>= 1;
    }

    /* Extract odd prime factors by trial division. */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        while (m % d == 0) {
            factors[cnt++] = d;
            m /= d;
        }
    }
    if (m > 1) {          /* any remaining prime factor > sqrt(original n) */
        factors[cnt++] = m;
    }

    /* cnt is guaranteed to be > 0 for n >= 2 */
    if ((cnt & 1ULL) == 1) {               /* odd number of factors */
        return (seq_t)factors[cnt / 2];
    } else {                               /* even number of factors */
        uint64_t a1 = factors[cnt / 2 - 1];
        uint64_t a2 = factors[cnt / 2];
        uint64_t sum = a1 + a2;

        if ((sum & 1ULL) == 0) {           /* sum even → integer median */
            return (seq_t)(sum >> 1);
        } else {                           /* sum odd → numerator of .5 fraction */
            return (seq_t)sum;
        }
    }
}
