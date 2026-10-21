/*
 * A262493  Centered 13-gonal (or tridecagonal) primes.
 * https://oeis.org/A262493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form (13*k^2 - 13*k + 2)/2 for k ≥ 1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

/* Return the n‑th centered 13‑gonal prime (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)              /* the offset is 1; values for n < 1 are undefined */
        return 0;

    idx_t     k        = 1;           /* current k value */
    idx_t     found    = 0;           /* number of primes found so far */

    while (1) {
        /* compute (13*k^2 - 13*k + 2)/2 using seq_t arithmetic only */
        seq_t num = (seq_t)13 * (seq_t)k * (seq_t)k
                  - (seq_t)13 * (seq_t)k
                  + (seq_t)2;
        seq_t val = num / 2;

        if (isPrimeGeneric(val)) {
            ++found;
            if (found == n)
                return val;           /* found the desired term */
        }
        ++k;
    }
}
