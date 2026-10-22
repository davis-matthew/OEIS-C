/*
 * A383809  Consecutive states of a linear congruential pseudo-random number
 *          generator for Lisp 1985 when started at 1.
 * https://oeis.org/A383809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17 * a(n-1) mod 251, a(1) = 1
 * Range:     u8 n <= 125, u16 n <= 125, u32 n <= 125, u64 n <= 125,
 *            u128 n <= 125, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes one period (125) of the sequence
   and returns the value for any n ≥ 1. */
#include <stddef.h>
#include <stdbool.h>

/* Period of the sequence modulo 251.  Must be a compile‑time constant. */
enum { PERIOD = 125 };

/* Storage for one full period.  seq_t is defined by the build. */
static seq_t period_values[PERIOD];
static bool period_ready = false;

/* Initialise the period table once.  Safe for any seq_t type. */
static void init_period(void)
{
    period_values[0] = 1;
    for (size_t i = 1; i < PERIOD; ++i)
    {
        period_values[i] = (period_values[i - 1] * 17) % 251;
    }
    period_ready = true;
}

/* Return the n‑th term (1‑based).  The function is correct for all
   n ≥ 1, independent of call order. */
seq_t a(idx_t n)
{
    if (!period_ready)
    {
        init_period();
    }

    /* n ≥ 1 by definition; use 0‑based index into the period. */
    idx_t pos = (n - 1) % PERIOD;
    return period_values[(size_t)pos];
}
