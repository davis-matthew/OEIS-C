/*
 * A352337  Numbers m such that A_m(j) = 1 for some value of j, where A_m(k+1)
 *          = 2*A_m(k) - sigma(A_m(k)), sigma(m) = sum of the divisors of m,
 *          and A_m(1) = m.
 * https://oeis.org/A352337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer m for which the iterated
   transformation A_m(k+1) = 2*A_m(k) – sigma(A_m(k)) eventually
   reaches 1, starting with A_m(1) = m.  The function searches for
   the required term by scanning integers in increasing order. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* Test whether m participates in the sequence.  For the small
   values that occur in the first few thousand terms, m and all
   intermediate values fit in idx_t, so casting to idx_t is safe
   for the sigma_of call. */
static bool qualifies(idx_t m)
{
    seq_t current = (seq_t)m;

    while (true) {
        /* sigma_of works on idx_t, but current is the seq_t
           value that must be cast to idx_t for the call. */
        seq_t sigma = sigma_of((idx_t)current);

        /* If current is not deficient, the sequence stops. */
        if (sigma >= (current + current))
            return false;

        /* Next value in the sequence. */
        seq_t next = (current + current) - sigma;
        if (next == (seq_t)1)
            return true;          /* reached 1 */
        if (next == (seq_t)0)
            return false;         /* should not happen, but safe */

        current = next;          /* continue iteration */
    }
}

/* Required entry point.  For any n >= 1 this returns the nth term
   of the sequence.  The function is independent on the order of
   calls. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; should not occur but harmless */
        return 0;

    idx_t found = 0;
    seq_t ans = 0;

    for (idx_t m = 1; found < n; ++m) {
        if (qualifies(m)) {
            ++found;
            ans = (seq_t)m;
        }
    }

    return ans;
}
