/*
 * A385893  Cycle of length 130 in the dynamical system A385938 starting from
 *          26.
 * https://oeis.org/A385893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 129, u64 n <= 129,
 *            u128 n <= 129, big any n
 * Helpers:   next_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A385893 (Cycle of length 130 in the dynamical system A385938).
 *
 * Recurrence: a(0) = 26; a(n) = f( a(n-1) ) for n ≥ 1,
 * where
 *   f(x) = (7*x+1)/3   if x mod 3 = 2,
 *   f(x) = (2*x+1)/3   if x mod 3 = 1,
 *   f(x) = (2*x)  /3   if x mod 3 = 0.
 *
 * The sequence eventually repeats every 130 terms; this routine
 * precomputes the cycle once and then returns the appropriate term.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/next_step.h"

/* sequence a(n) */
seq_t a(idx_t n)
{
    /* 130-term cycle – known constant */
    static seq_t cycle[130];
    static bool initialized = false;

    if (!initialized) {
        cycle[0] = (seq_t)26;
        for (idx_t i = 1; i < 130; ++i)
            cycle[i] = next_step(cycle[i - 1]);
        initialized = true;
    }

    idx_t idx = n % 130;
    return cycle[idx];
}
