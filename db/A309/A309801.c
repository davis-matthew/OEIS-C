/*
 * A309801  If 2*n = Sum (2^e_k) then a(n) = Sum (e_k^n).
 * https://oeis.org/A309801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ (k+1)^n over all bit positions k where bit k of n is 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;            /* accumulator for the sum */
    idx_t bit = 0;            /* current bit index (0‑based) */
    idx_t tmp = n;            /* working copy of n */

    /* iterate over set bits of n */
    while (tmp != 0)
    {
        if (tmp & 1ULL)  /* bit k is set */
        {
            seq_t base = (seq_t)(bit + 1);        /* (k + 1) */
            seq_t term = ipow(base, (seq_t)n); /* (k + 1)^n */
            sum += term;                         /* accumulate */
        }
        tmp >>= 1;      /* move to next bit */
        ++bit;
    }
    return sum;
}
