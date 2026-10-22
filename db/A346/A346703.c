/*
 * A346703  Product of primes at odd positions in the weakly increasing list
 *          (with multiplicity) of prime factors of n.
 * https://oeis.org/A346703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = product of primes at odd positions (1‑indexed) in * the
 *            weakly increasing multiset of prime factors of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;
    int pos = 0;                /* 0‑based position in the factor list */

    /* factor 2 */
    while ((temp & 1ULL) == 0) {
        if ((pos & 1) == 0) prod *= (seq_t)2;
        pos++;
        temp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        while (temp % p == 0) {
            if ((pos & 1) == 0) prod *= (seq_t)p;
            pos++;
            temp /= p;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (temp > 1) {
        if ((pos & 1) == 0) prod *= (seq_t)temp;
    }

    return prod;
}
