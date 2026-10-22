/*
 * A382248  Smallest number k that is neither squarefree nor a prime power
 *          such that k is coprime to n.
 * https://oeis.org/A382248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p^2 * q where p is the smallest prime coprime to n * and
 *            q is the second smallest prime coprime to n
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

/* Return the n-th term of OEIS sequence A382248.  */
seq_t a(idx_t n)
{
    uint64_t p1 = 0, p2 = 0;
    uint64_t cand = 2;

    /* Find the first two primes that are coprime to n. */
    while (p1 == 0 || p2 == 0) {
        if (is_prime(cand)) {
            if (gcd((seq_t)n, (seq_t)cand) == (seq_t)1) {
                if (p1 == 0)
                    p1 = cand;
                else if (p2 == 0)
                    p2 = cand;
            }
        }
        ++cand;
    }

    /* Compute p^2 * q in the representation seq_t. */
    seq_t result = (seq_t)p1 * (seq_t)p1;
    result *= (seq_t)p2;
    return result;
}
