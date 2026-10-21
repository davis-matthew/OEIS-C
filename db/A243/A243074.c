/*
 * A243074  a(1) = 1, a(n) = n/p^(k-1), where p = largest prime dividing n and
 *          p^k = highest power of p dividing n.
 * https://oeis.org/A243074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / p^(k-1), where p is the largest prime dividing n and
 *            p^k is the highest power of p dividing n
 * Range:     u8 n <= 255, u16 n <= 9591, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS A243074 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;      /* a(1) = 1 */

    /* Find the largest prime factor of n */
    uint64_t temp = (uint64_t)n;
    uint64_t largest = 1;
    uint64_t d = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            largest = d;
            while (temp % d == 0)
                temp /= d;
        }
        d = (d == 2) ? 3 : d + 2;   /* iterate over odd numbers after 2 */
    }
    if (temp > 1)
        largest = temp;            /* temp is a prime larger than sqrt(original n) */

    /* Count the exponent of the largest prime factor in the original n */
    uint64_t exponent = 0;
    uint64_t rem = (uint64_t)n;
    while (rem % largest == 0) {
        rem /= largest;
        exponent++;
    }

    /* Compute the denominator p^(k-1) */
    seq_t denom = 1;
    if (exponent > 1) {            /* exponent-1 times */
        seq_t p = (seq_t)largest;
        for (uint64_t i = 1; i < exponent; ++i)
            denom *= p;
    }

    /* a(n) = n / denom */
    return ((seq_t)n) / denom;
}
