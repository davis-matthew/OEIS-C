/*
 * A353511  Positions of odd terms in A001001, where A001001(n) = Sum_{d|n}
 *          d*sigma(d).
 * https://oeis.org/A353511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_odd_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A353511
 *  Positions of odd terms in A001001(n) = Σ_{d|n} d * σ(d)
 *  Criterion: n yields an odd A001001(n) iff every odd prime exponent in n
 *  is congruent to 0 or 1 modulo 4.  The function a(n) returns the nth
 *  integer with this property (1‑based index). */
 
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_odd_term.h"

/* Main sequence function.  Linear search suffices for n up to ≈10000. */
seq_t a(idx_t n) {
    idx_t count = 0;           /* how many qualifying numbers seen */
    idx_t cur   = 1;           /* current integer to test */

    while (1) {
        if (is_odd_term(cur)) {
            count++;
            if (count == n)
                return (seq_t)cur;   /* conversion works for both integer and big types */
        }
        cur++;
    }
}
