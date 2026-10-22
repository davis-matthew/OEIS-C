/*
 * A320860  Powers of 2 with initial digit 4.
 * https://oeis.org/A320860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Powers of 2 with initial digit 4.
   The function keeps a static cache of exponents e such that the first
   decimal digit of 2^e is 4.  The nth term is 2^e[n-1].  Exponents are
   identified by iteratively adding log10(2) modulo 1 and testing if the
   fractional part lies in [log10(4), log10(5)). */

#include <math.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* unused, safeguard */
    static size_t sz = 0;                   /* number of found exponents */
    static size_t cap = 0;                  /* current capacity of exp array */
    static idx_t *exps = NULL;              /* stored exponents e_1..e_sz */
    static idx_t next_exp = 1;              /* next exponent to test */
    static long double frac = 0.0L;         /* fractional part of exponent*log10(2) */

    const long double log10_2 = log10l(2.0L);
    const long double log10_4 = log10l(4.0L);
    const long double log10_5 = log10l(5.0L);
    const long double eps   = 1e-12L;        /* safety margin for rounding */

    /* Build the cache up to the n-th term if needed */
    while (sz < n) {
        /* Advance the fractional part */
        frac += log10_2;
        if (frac >= 1.0L) {
            frac -= floorl(frac);
            if (frac >= 1.0L) frac -= 1.0L; /* guard against tiny overshoot */
        } else if (frac < 0.0L) {
            frac += 1.0L;
        }

        /* Check whether the leading digit is 4 */
        if (frac + eps >= log10_4 && frac + eps < log10_5) {
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                idx_t *tmp = (idx_t *)realloc(exps, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                exps = tmp;
                cap = new_cap;
            }
            exps[sz++] = next_exp;
        }

        ++next_exp;
    }

    /* Return the 2^exponent value.  pow2 handles arbitrary‑precision if needed. */
    return pow2(exps[n - 1]);
}
