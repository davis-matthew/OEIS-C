/*
 * A271367  Primes congruent to 11, 13, 17 or 19 (mod 30).
 * https://oeis.org/A271367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   this function returns the n-th prime congruent to 11, 13, 17 or
 *            19 modulo 30, i.e. sequence A271367 from OEIS
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t x); */

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t candidate = 11;      /* first possible prime in the list */

    while (1) {
        idx_t r = candidate % 30;
        if (r == 11 || r == 13 || r == 17 || r == 19) {
            if (is_prime(candidate)) {
                ++count;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        ++candidate;
    }
}
