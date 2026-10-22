/*
 * A353629  a(n) = 1 if n is a product of an even number of distinct primes,
 *          otherwise 0.
 * https://oeis.org/A353629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is square‑free with an even number of distinct
 *            primes, otherwise 0. This is equivalent to a(n) = 1 iff μ(n) =
 *            +1, where μ is the Möbius function
 * Range:     u8 n <= 9646, u16 n <= 9646, u32 n <= 9646, u64 n <= 9646,
 *            u128 n <= 9646, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
 
seq_t a(idx_t n) {
    /* n is in [1, UINT64_MAX], but we treat it as a 64‑bit unsigned integer. */
    uint64_t m = (uint64_t)n;
    int distinct = 0;          /* count of distinct prime factors */
 
    /* Trial‑division factorization.  This is efficient for n up to about 10^12
       and far below 10000 as required by the time limit. */
    for (uint64_t p = 2; p <= m / p; ++p) { /* use m/p to avoid overflow of p*p */
        if (m % p == 0) {
            int exp = 0;
            do {
                m /= p;
                ++exp;
            } while (m % p == 0);
            /* If any prime factor occurs more than once, n is not square‑free. */
            if (exp > 1) {
                return (seq_t)0;
            }
            ++distinct;
        }
    }
    /* Remaining factor m > 1 is prime and appears once. */
    if (m > 1) {
        ++distinct;
    }
 
    /* Return 1 if the number of distinct prime factors is even,
       0 otherwise.  The case n==1 gives distinct==0, hence 1. */
    return (seq_t)((distinct & 1) == 0 ? 1 : 0);
}
