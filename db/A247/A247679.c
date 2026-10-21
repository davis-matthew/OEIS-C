/*
 * A247679  Composite numbers congruent to 17 modulo 18.
 * https://oeis.org/A247679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth composite integer congruent to 17 mod 18
 * Range:     u8 n <= 5, u16 n <= 2547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches successive numbers of the form 18*k+17
   and returns the n-th one that is composite (i.e., not prime). */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Safety: for n < 1 the sequence is not defined (offset 1) */
    if (n == 0) return 0;

    idx_t count = 0;   /* how many found so far */
    idx_t k = 1;       /* current multiplier of 18 */

    while (count < n) {
        /* candidate value: 18*k + 17 */
        seq_t val = (seq_t)18 * (seq_t)k + (seq_t)17;

        /* check if it is not a prime -> thus composite (since >1) */
        if (!is_prime((idx_t)val)) {
            count++;
            if (count == n)
                return val;
        }
        k++;
    }

    /* Should never be reached */
    return 0;
}
