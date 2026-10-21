/*
 * A285800  Numbers having more than one odd prime factor to an odd power in
 *          their prime factorization.
 * https://oeis.org/A285800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer having at least two distinct odd prime
 *            factors that occur to an odd exponent in its prime factorization
 * Range:     u8 n <= 85, u16 n <= 9468, u32 n <= 9468, u64 n <= 9468,
 *            u128 n <= 9468, big any n
 * Helpers:   has_two_odd_prime_odds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_two_odd_prime_odds.h"

seq_t a(idx_t n) {
    idx_t found   = 0;    /* number of terms found so far */
    idx_t candidate = 1;  /* current integer to test */

    while (found < n) {
        candidate++;               /* test next integer */
        if (has_two_odd_prime_odds(candidate)) {
            found++;               /* one more term found */
        }
    }

    return (seq_t)candidate;       /* the n-th term */
}
