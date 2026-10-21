/*
 * A247678  Odd composite numbers congruent to 4 modulo 9.
 * https://oeis.org/A247678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th odd composite number congruent to 4 (mod 9)
 * Range:     u8 n <= 5, u16 n <= 2548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"   /* provides bool isPrimeTrial(seq_t n) */

/* Return the n-th odd composite ≡ 4 (mod 9).  Indexing starts at 1. */
seq_t a(idx_t n)
{
    /* All such numbers are of the form 18*m + 13 with m odd (m = 2,3,4,…).
       The smallest composite occurs at m = 2 (49). */
    idx_t m = 2;                 /* current m value (candidate = 18*m+13) */
    idx_t count = 0;             /* number of composites found so far */
    seq_t candidate;

    while (count < n) {
        candidate = (seq_t)18 * m + (seq_t)13;
        /* candidate is odd and > 1; test for primality. */
        if (!isPrimeTrial(candidate)) {
            /* Non‑prime ⇒ composite (odd by construction). */
            count++;
            if (count == n)
                return candidate;
        }
        m++;
    }
    return 0;   /* should never happen since n ≥ 1 */
}
