/*
 * A276471  Numbers m of the form 2*k^2 - 61 which are +-1 times a prime.
 * https://oeis.org/A276471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276471: Numbers m of the form 2*k^2 - 61 which are ±1 times a prime.
 * a(n) is the nth such integer (1‑based), a(1) = -61.  The definition is used
 * directly by iterating over k = 0,1,2,…, testing |m| for primality.  The
 * implementation is O(n) and requires no caching. */
#include <stdbool.h>
#include "../utils/is_prime_small.h"

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    idx_t k = 0;          /* current k value */
    idx_t found = 0;      /* how many terms found so far */

    while (found < n) {
        seq_t kk = (seq_t)k;                  /* cast to seq_t for arithmetic */
        seq_t m = (seq_t)2 * kk * kk - (seq_t)61;  /* m = 2*k^2 - 61 */

        seq_t abs_m = (m >= 0) ? m : -m;    /* |m| for primality test */

        /* Test if |m| is prime; is_prime_small works for both small and
         * arbitrary‑precision seq_t values. */
        if (is_prime_small(abs_m)) {
            found++;
            if (found == n)
                return m;                    /* desired term found */
        }

        k++;                                 /* move to next k */
    }

    /* Control never reaches here for n >= 1. */
    return 0;
}
