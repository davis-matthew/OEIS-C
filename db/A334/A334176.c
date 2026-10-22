/*
 * A334176  Squarefree part of numerator of the squared area of the Heronian
 *          triangle with sequential odd sides whose shortest leg is 2*n+1.
 * https://oeis.org/A334176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree part of 3*(2*n+7)*(2*n-1) i.e. product of
 *            primes dividing 3*(2*n+7)*(2*n-1) to an odd power
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute a(n) for any n >= 1.
    Uses only integer arithmetic and a simple trial‑division factorisation.   */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute the integer whose square‑free part we need.               */
    uint64_t val = 3 * ((uint64_t)2 * n + 7) * ((uint64_t)2 * n - 1);
    uint64_t rem = val;
    seq_t   prod = 1;

    /* Handle factor 2 separately.                                          */
    if (rem % 2 == 0) {
        unsigned cnt = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            cnt++;
        }
        if (cnt & 1) {
            prod *= 2;
        }
    }

    /* Trial division by odd primes up to sqrt(rem).                        */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            unsigned cnt = 0;
            while (rem % p == 0) {
                rem /= p;
                cnt++;
            }
            if (cnt & 1) {
                prod *= p;
            }
        }
    }

    /* Any remaining factor is prime and appears to an odd power (exactly 1). */
    if (rem > 1) {
        prod *= rem;
    }

    return prod;
}
