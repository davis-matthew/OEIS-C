/*
 * A290838  a(n) = smallest prime p such that 2p - 2n + 1 is prime.
 * https://oeis.org/A290838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that 2p-2n+1 is prime
 * Range:     u8 n <= 228, u16 n <= 9922, u32 n <= 9922, u64 n <= 9922,
 *            u128 n <= 9922, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Compute the nth term of OEIS A290838.
 * a(n) is the smallest prime p > n for which 2p-2n+1 is also prime. */
seq_t a(idx_t n)
{
    for (idx_t m = 1;; ++m) {                 /* iterate over potential p = n+m */
        seq_t p = (seq_t)n + (seq_t)m;        /* candidate prime p */
        if (!isPrimeGeneric(p))
            continue;                        /* p must be prime */

        seq_t q = 2 * (seq_t)m + (seq_t)1;    /* 2p-2n+1 = 2m+1 */
        if (isPrimeGeneric(q))
            return p;                        /* found the smallest suitable prime */
    }
}
