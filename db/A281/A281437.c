/*
 * A281437  Primes of the form 25*n^2 + 25*n + 47.
 * https://oeis.org/A281437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 4390, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th prime of the form 25*k^2 + 25*k + 47
   (k starts at 0).  The function tests successive k, checks for
   primality, and returns the value when the n-th prime is found. */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        seq_t val = (seq_t)25 * (seq_t)k * (seq_t)k
                  + (seq_t)25 * (seq_t)k
                  + (seq_t)47;
        if (isPrimeGeneric(val)) {
            count++;
            if (count == n)
                return val;
        }
        k++;
    }
}
