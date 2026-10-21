/*
 * A284600  a(n) = n/(largest prime power dividing n).
 * https://oeis.org/A284600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/(largest prime power dividing n)
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest k such that n/k is a prime power,
   i.e. n divided by the largest p^e|n. */

#include <stdint.h>

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, but safe */

    uint64_t nn   = (uint64_t)n;   /* original n */
    uint64_t temp = nn;            /* value to factor */
    uint64_t max_pp = 1;           /* largest prime power found */

    while (temp > 1) {
        uint64_t divisor = 0;

        /* find smallest divisor of temp */
        if ((temp & 1U) == 0) {
            divisor = 2;
        } else {
            uint64_t d;
            for (d = 3; d * d <= temp; d += 2) {
                if (temp % d == 0) { divisor = d; break; }
            }
            if (divisor == 0) divisor = temp;   /* temp is prime */
        }

        /* compute p^e for this divisor */
        uint64_t pp = divisor;
        temp /= divisor;
        while (temp % divisor == 0) {
            temp /= divisor;
            pp *= divisor;
        }

        if (pp > max_pp) max_pp = pp;
    }

    return (seq_t)(nn / max_pp);
}
