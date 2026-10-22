/*
 * A348007  Starting value of the longest run of halving steps in the
 *          trajectory from n to 1 in the Collatz map (or 3x+1 problem), or -1
 *          if no such trajectory exists.
 * https://oeis.org/A348007
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simulate the Collatz trajectory of n to 1 and
   identify the starting value of the longest consecutive chain of
   halving steps.  If several chains have the same maximum length,
   the highest starting value is returned.  If the trajectory does not
   reach 1 within a generous step limit, -1 is returned. */
#include <stdint.h>
#include <stdbool.h>

#define MAX_STEPS 50000ULL

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* offset starts at 2 */

    seq_t current = (seq_t)n;
    seq_t best_start = 0;
    unsigned best_len = 0;
    uint64_t steps = 0;

    while (current != (seq_t)1)
    {
        if (steps++ > MAX_STEPS)          /* safety against non‑converging inputs */
            return (seq_t)-1;

        if ((current & (seq_t)1) == 0)    /* even: a halving run starts here */
        {
            seq_t run_start = current;
            unsigned run_len = 0;

            while ((current & (seq_t)1) == 0)
            {
                current >>= 1;            /* divide by 2 */
                run_len++;
            }

            if (run_len > best_len ||
                (run_len == best_len && run_start > best_start))
            {
                best_len = run_len;
                best_start = run_start;
            }
        }
        else                              /* odd: apply the 3x+1 rule */
        {
            current = current * (seq_t)3 + (seq_t)1;
        }
    }

    return best_start;                    /* best_start is guaranteed > 0 for n≥2 */
}
