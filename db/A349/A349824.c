/*
 * A349824  a(0) = 0; for n >= 1, a(n) = (number of prime factors, counted
 *          with repetition) * (sum of prime factors, counted with
 *          repetition).
 * https://oeis.org/A349824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of prime factors of n, with multiplicity) * *
 *            (sum of prime factors of n, with multiplicity)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* sequence element computation */
seq_t a(idx_t n)
{
    /* By definition a(0) = a(1) = 0 */
    if (n <= 1)
        return 0;

    /* Work with a 64‑bit integer for factoring */
    uint64_t m = (uint64_t)n;
    seq_t omega = 0;   /* number of prime factors (with multiplicity)   */
    seq_t sum_pf = 0;  /* sum of the prime factors (with multiplicity) */

    /* factor 2 separately to keep the loop odd */
    while ((m & 1ULL) == 0) {
        omega++;
        sum_pf += (seq_t)2;
        m >>= 1;
    }

    /* trial division for odd divisors */
    for (uint64_t d = 3; d <= (m / d); d += 2) {
        while (m % d == 0) {
            omega++;
            sum_pf += (seq_t)d;
            m /= d;
        }
    }

    /* if anything remains it is a prime factor > sqrt(n) */
    if (m > 1) {
        omega++;
        sum_pf += (seq_t)m;
    }

    return omega * sum_pf;
}
