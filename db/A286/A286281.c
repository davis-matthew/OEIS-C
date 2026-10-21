/*
 * A286281  a(n) = floor the elevator is on at the n-th stage of Ken
 *          Knowlton's elevator problem, version 2.
 * https://oeis.org/A286281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2704, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A286281 – Ken Knowlton's elevator problem, version 2.
 *
 * Simulation rule:
 *   • The elevator starts at floor 1 and always goes up from floor 1.
 *   • For a floor m > 1, the elevator steps up on the m‑th
 *     counted stop at that floor.  Only stops that come after an
 *     upward move count; stops reached while descending do not
 *     increment the counter.  On all other stops it steps down.
 *   • Floor 1 always causes an upward step.
 *
 * a(n) is defined as the floor the elevator occupies at the n‑th
 * (1‑based) stop.  The algorithm below simulates the process
 * directly.  It runs in O(n) time and O(n) memory and works for
 * any integer type chosen for seq_t/idx_t by the build system.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate one counter for each possible floor.  Floor values
       never exceed n+1 for the required range (up to about 10 000). */
    size_t sz = (size_t)(n + 2);
    seq_t *counters = (seq_t *)malloc(sizeof(seq_t) * sz);
    if (!counters) return (seq_t)0;                 /* allocation failure */
    for (idx_t i = 0; i < sz; ++i) counters[i] = 0;

    idx_t floor = 1;          /* current floor at the current stop */
    int    dir   = 1;          /* direction of the previous move: +1 up, -1 down */

    /* We perform n‑1 transitions (after stops 1…n‑1).  After the loop,
       `floor` is the floor at stop n. */
    for (idx_t step = 1; step < n; ++step)
    {
        bool counted = (dir == 1);          /* counted only if arrived from below */

        if (floor == 1)                     /* special handling for floor 1 */
        {
            if (counted) ++counters[1];
            dir = 1;                        /* always go up from floor 1 */
        }
        else
        {
            if (counted)
            {
                ++counters[floor];
                if (counters[floor] % (seq_t)floor == 0)
                    dir = 1;                /* go up on the m‑th counted stop */
                else
                    dir = -1;               /* otherwise down */
            }
            else
                dir = -1;                   /* descending stop – always down */
        }

        floor += dir;                       /* move to the next floor */
    }

    free(counters);
    return floor;                            /* a(n) */
}
