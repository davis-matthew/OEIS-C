/*
 * A268689  Let f(n) = maximal value of the weak Goodstein function g_k(n) for
 *          k >= 0; then a(n) = minimal value of k such that g_k(n) = f(n).
 * https://oeis.org/A268689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = minimal k such that the weak Goodstein function g_k(n)
 *            attains its maximal value f(n). The algorithm iteratively
 *            evaluates base representations of n with increasing bases until
 *            the sequence g_k(n) becomes non‑increasing; the first index at
 *            which the maximum is attained is returned
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   weak_goodstein_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This file may be compiled as C11 or C++17.  The type seq_t is
   predefined by the build system as an unsigned integer of any width
   or an arbitrary‑precision integer class; the same code works in
   both languages. */
#include <stdint.h>
#include <stdbool.h>   /* required for bool, though not used */
#include "../utils/weak_goodstein_step.h"

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    seq_t cur      = (seq_t)n;          /* current g_k value       */
    seq_t max_val  = cur;               /* best value so far      */
    seq_t max_k    = (seq_t)0;           /* minimal k that attains
                                           max_val                  */

    for (idx_t k = 1; ; ++k) {
        if (cur == (seq_t)0)            /* 0 yields nothing larger   */
            break;

        seq_t next = weak_goodstein_step(cur, (idx_t)(k + 1));

        if (next > max_val) {           /* new maximum found        */
            max_val = next;
            max_k   = (seq_t)k;
        }

        if (next <= cur)                /* sequence has plateaued or
                                           started decreasing      */
            break;

        cur = next;                     /* advance to next g_k       */
    }

    return max_k;
}
