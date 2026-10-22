/*
 * A382615  Expansion of 1/(1 - x/(1 - x)^3)^3.
 * https://oeis.org/A382615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(n+2*k-1,n-k)
 *            Recurrence: a(n) = 12*a(n-1) - 57*a(n-2) + 139*a(n-3) -
 *            195*a(n-4) + 174*a(n-5) - 102*a(n-6) + 39*a(n-7) - 9*a(n-8) +
 *            a(n-9) Initial values: a(0)=1, a(1)=3, a(2)=15, a(3)=64,
 *            a(4)=261, a(5)=1032, a(6)=3982, a(7)=15066, a(8)=56094,
 *            a(9)=2...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* dynamic cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated size of cache */
    static idx_t computed_upto = 0;  /* cache contains indices 0 .. computed_upto-1 */

    /* enlarge cache if required */
    if ((idx_t)(cap) <= n) {
        grow_seq(&cache, &cap, (size_t)n + 1);
    }

    /* if needed, compute all terms up to n in order */
    for (idx_t i = computed_upto; i <= n; ++i) {
        if (i <= 9) {   /* initial values */
            /* these are the first ten terms; they fit into seq_t for the
               supported integer widths (8/16/32/64/128) if the test values
               stay within the range; larger widths use big integers. */
            static const seq_t init[10] = {
                1,   3,   15,  64,  261,
                1032, 3982, 15066, 56094, 206068
            };
            cache[i] = init[i];
        } else {      /* recurrence of order 9 */
            seq_t term = cache[i-1] * 12;
            term -= cache[i-2] * 57;
            term += cache[i-3] * 139;
            term -= cache[i-4] * 195;
            term += cache[i-5] * 174;
            term -= cache[i-6] * 102;
            term += cache[i-7] * 39;
            term -= cache[i-8] * 9;
            term += cache[i-9];
            cache[i] = term;
        }
    }
    computed_upto = n + 1;   /* all indices up to n are now computed */
    return cache[n];
}
