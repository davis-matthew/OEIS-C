/*
 * A350496  Positive integers k such that if p is the largest prime less than
 *          k then k - p is prime.
 * https://oeis.org/A350496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive integers k such that if p is the largest prime less
   than k then k - p is prime.  a(n) is the n-th such integer, with
   offset 1 (a(1) = 5). */
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Computes the n-th term of OEIS A350496.  The function works for any
   integer type seq_t (fixed-size or arbitrary-precision) because it
   only uses the `is_prime` routine on 64‑bit indices and converts
   the resulting 64‑bit value to seq_t on return. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for 0 is irrelevant */

    idx_t count = 0;        /* number of terms found so far */
    idx_t k = 2;            /* current integer being examined */
    idx_t last_prime = 2;   /* largest prime < k */

    while (count < n) {
        k++;                                 /* next integer to test */

        bool k_is_prime = is_prime(k);       /* prime status of k */

        idx_t diff = k - last_prime;         /* k - largest prime < k */
        if (is_prime(diff)) {
            count++;
            if (count == n)
                return (seq_t)k;             /* found the required term */
        }

        if (k_is_prime) last_prime = k;      /* update the latest prime */
    }

    /* unreachable: function always returns before reaching here */
    return 0;
}
