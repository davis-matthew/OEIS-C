/*
 * A379964  a(n) = 1 if (n^2)-1 is not divisible by p^p for any prime p,
 *          otherwise 0.
 * https://oeis.org/A379964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 7864, u32 n <= 7864, u64 n <= 7864,
 *            u128 n <= 7864, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A379964
 * Formula: a(n) = 1 if (n^2)-1 is NOT divisible by p^p for any prime p, else 0
 * (using integer arithmetic only, as required by the build)
 */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition (0 is divisible by every p^p) */
    if (n <= 1) return 0;

    /* m = n^2 - 1   (seq_t arithmetic, no overflow on arbitrary‑precision builds) */
    seq_t m = (seq_t)n * (seq_t)n;
    m -= 1;

    /* Test primes in increasing order until p^p exceeds m.
       Once p^p > m, no larger prime can satisfy p^p | m,
       so the loop may stop. */
    for (idx_t p = 2; ; ++p) {
        if (!is_prime(p)) continue;          /* skip non‑primes */

        /* Compute p^p into pp, but stop early if it exceeds m
           (this avoids needless large intermediate values). */
        seq_t pp = 1;
        bool over = false;
        for (idx_t i = 0; i < p; ++i) {
            pp *= p;
            if (pp > m) { over = true; break; }
        }

        if (over) break;          /* p^p too large, so all larger primes are also too large */

        /* If m is divisible by p^p, the condition fails */
        if ((m % pp) == 0) return 0;
    }

    /* No prime p satisfied p^p | (n^2 - 1) */
    return 1;
}
