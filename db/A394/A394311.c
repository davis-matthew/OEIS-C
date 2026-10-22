/*
 * A394311  a(n) is the smallest prime p for which ((n+1)*p - 1)/n is also
 *          prime.
 * https://oeis.org/A394311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 315, u32 n <= 2212, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A394311.
   a(n) is the smallest prime p such that
        ((n+1)*p - 1)/n
   is also prime.
   By algebra p must be ≡ 1 (mod n), so we search primes of the form
   p = 1 + k*n (k ≥ 1) and test the condition.   */
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* n as seq_t */
    if (nn == 0) return 0;        /* undefined for offset, harmless */

    seq_t inc = nn;               /* step size n */
    seq_t p = 1 + inc;            /* first candidate (k = 1) */

    /* search for the smallest qualifying prime */
    while (true) {
        if (is_prime_big(p)) {
            seq_t q = ((nn + 1) * p - 1) / nn;
            if (is_prime_big(q))
                return p;
        }
        p += inc;                 /* next candidate: p = 1 + (k+1)*n */
    }
}
