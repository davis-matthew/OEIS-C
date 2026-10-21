/*
 * A267759  Numbers of the form 10k+u where 0 <= u <= 9 and k has a digit > u
 *          or a digit 1 followed by a digit < u.
 * https://oeis.org/A267759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   k_has_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer of the form 10k+u (0 ≤ u ≤ 9) such that
   either k has a digit > u or k contains a digit 1 immediately followed by
   a digit < u.  The function is constructed to work with any integer
   type seq_t supplied by the build (native or arbitrary‑precision). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/k_has_condition.h"

/* OEIS sequence A267759 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0 to keep the function type sound. */
        return (seq_t)0;
    }

    idx_t count = 0;           /* number of qualifying terms found so far */
    uint64_t k = 1;            /* integer part of the candidate number */

    while (1) {
        for (int u = 0; u < 10; ++u) {
            if (k_has_condition(k, u)) {
                ++count;
                if (count == n) {
                    return (seq_t)10 * (seq_t)k + (seq_t)u;
                }
            }
        }
        ++k;   /* proceed to the next integer k */
    }
}
