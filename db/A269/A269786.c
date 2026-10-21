/*
 * A269786  Primes p such that 2*p + 31 is a square.
 * https://oeis.org/A269786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 21, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269786: primes p such that 2*p + 31 is a square.
   Equivalent form: p = 2*k^2 + 2*k - 15.
   This implementation searches for the n‑th such prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = (seq_t)1;

    while (found < n) {
        /* p = 2*k^2 + 2*k - 15 */
        seq_t p = (seq_t)2 * k * k + (seq_t)2 * k - (seq_t)15;
        if (p >= (seq_t)2 && is_prime_big(p)) {
            ++found;
            if (found == n)
                return p;
        }
        ++k;
    }
    /* Should never reach here for valid n */
    return (seq_t)0;
}
