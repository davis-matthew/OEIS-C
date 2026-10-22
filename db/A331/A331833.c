/*
 * A331833  Numbers k such that all the divisors of k, excluding 1, have an
 *          even number of 1's in their negabinary representations.
 * https://oeis.org/A331833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that every divisor of k
 *            * greater than 1 has an even number of 1’s in its negabinary
 *            (base –2) * representation. Sequence is zero‑offset (a(1)=1).
 *            The function * recomputes the term from scratch on each call. It
 *            works for any n * that does not require k > U...
 * Range:     u8 n <= 38, u16 n <= 6309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   evensize_negabinary_ones_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//=============================================================================

#include <stdint.h>
#include <stdbool.h>
#include "../utils/evensize_negabinary_ones_u64.h"

/* Test whether every divisor d > 1 of k has an even number of 1’s in its
 * negabinary representation.  For k=1 the property is satisfied by definition. */
static bool good_candidate(uint64_t k)
{
    if (k <= 1) return true;                     /* 1 itself is a term */

    /* Check all divisors d from 2 up to sqrt(k).  Use k/d in the loop
     * condition to avoid overflow of d*d. */
    for (uint64_t d = 2; d <= k / d; ++d) {
        if (k % d == 0) {
            if (!evensize_negabinary_ones_u64(d))
                return false;
            uint64_t e = k / d;
            if (e != d && !evensize_negabinary_ones_u64(e))
                return false;
        }
    }

    /* The divisor k itself must also have even number of 1’s. */
    return evensize_negabinary_ones_u64(k);
}

/* ------------------------------------------------------------------------- */
/* Main function required by the task.  The function is re‑entrant and does
 * not keep any static state between calls.  It simply counts candidates
 * until the n‑th valid one is found. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                         /* no definition for n=0 */

    uint64_t candidate = 1;
    uint64_t found = 0;

    while (true) {
        if (good_candidate(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
