/*
 * A262084  Numbers m that satisfy the equation phi(m + 6) = phi(m) + 6 where
 *          phi(m) = A000010(m) is Euler's totient function.
 * https://oeis.org/A262084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 2059, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A262084: m such that φ(m + 6) = φ(m) + 6
   The function searches for solutions in increasing order,
   stores them in a static array, and returns the n‑th one. */

#include <stdlib.h>
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *solutions = NULL;
    static size_t sol_cap = 0, sol_size = 0;
    static idx_t last_checked = 1;   /* next m to test */

    /* Ensure we have at least n solutions */
    while (sol_size < n)
    {
        seq_t m      = (seq_t)last_checked;
        seq_t phi_m  = phi(last_checked);          /* phi(m)  */
        seq_t phi_m6 = phi(last_checked + 6);      /* phi(m+6) */

        /* Check the condition φ(m+6) = φ(m) + 6 */
        if (phi_m6 == phi_m + (seq_t)6)
        {
            /* Grow the solutions array if needed */
            if (sol_size >= sol_cap)
            {
                size_t new_cap = sol_cap ? sol_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(solutions, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* Out of memory */
                solutions = tmp;
                sol_cap = new_cap;
            }
            solutions[sol_size++] = m;    /* Store the new solution */
        }
        /* Advance to next candidate */
        ++last_checked;
    }

    return solutions[n - 1];
}
