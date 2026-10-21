/*
 * A264825  Centered 18-gonal (or octadecagonal) primes.
 * https://oeis.org/A264825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes of the form 9*k^2 - 9*k + 1
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 32, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A264825 – centered 18‑gonal (octadecagonal) primes */

/* The value is obtained by searching successive k until the nth prime of the form 9*k^2-9*k+1 is found. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th centered 18-gonal prime.
   a(1) = 19, a(2) = 109, ... */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, but benign */

    idx_t found = 0;       /* number of primes discovered so far */

    /* iterate k from 2 upward, generating candidates of the form 9*k^2 - 9*k + 1 */
    for (idx_t k = 2;; ++k)
    {
        /* Convert k to the sequence type before performing arithmetic. */
        seq_t kk    = (seq_t)k;
        seq_t value = (seq_t)9 * kk * kk - (seq_t)9 * kk + (seq_t)1;

        if (is_prime_big(value))
        {
            ++found;
            if (found == n)
                return value;
        }
    }
}
