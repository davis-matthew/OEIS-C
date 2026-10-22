/*
 * A315655  Coordination sequence Gal.6.644.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A315655
   a(0) = 1
   a(1) = 6                     // first step is +5
   for n >= 2:
       a(n) = a(n-1) + (special(n-1) ? 4 : 6)
   where special(k) is true for k in
       {2, 8, 13, 19, 24, 30, 35, 41, …}
   The special indices start at 2 and then alternate
   steps of +6 and +5 (i.e., 2, 2+6, 2+6+5, 2+6+5+6, …). */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t val = 1;                  /* a(0) */
    val += 5;                       /* a(1) = 6 (first step +5) */

    /* Generate the special indices sequence while iterating */
    idx_t next_special = 2;          /* first special step index (k = 2) */
    int toggle = 0;                  /* alternates 0→6, 1→5 and back */

    for (idx_t i = 1; i < n; ++i)    /* i is k in a(k+1)-a(k) */
    {
        seq_t diff = (i == next_special) ? 4 : 6;
        val += diff;

        if (i == next_special) {
            /* Advance to the next special index */
            next_special += (toggle ? 5 : 6);
            toggle ^= 1; /* toggle between 0 and 1 */
        }
    }
    return val;
}
