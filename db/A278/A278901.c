/*
 * A278901  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 107", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A278901
   Formula: a(n) = 5*a(n-2) - 20*a(n-6) + 16*a(n-8)  for n > 15
   Initial values:
   a(0)  = 1
   a(1)  = 2
   a(2)  = 4
   a(3)  = 15
   a(4)  = 4
   a(5)  = 59
   a(6)  = 0
   a(7)  = 239
   a(8)  = 80
   a(9)  = 943
   a(10) = 320
   a(11) = 3839
   a(12) = 1344
   a(13) = 15039
   a(14) = 5376
   a(15) = 60415
*/

#include <stddef.h>

/* Compute the n-th term of the sequence A278901. */
seq_t a(idx_t n)
{
    /* Base values for n = 0 .. 15.  Use a fixed array to avoid dynamic memory and
       to keep the algorithm purely iterative.  The values are the known start of
       the sequence; the recurrence is applied only for n > 15. */
    static const seq_t base[16] = {
        (seq_t)1,  (seq_t)2,  (seq_t)4,  (seq_t)15,
        (seq_t)4,  (seq_t)59, (seq_t)0,  (seq_t)239,
        (seq_t)80, (seq_t)943, (seq_t)320, (seq_t)3839,
        (seq_t)1344, (seq_t)15039, (seq_t)5376, (seq_t)60415
    };

    if ((idx_t)0 <= n && n < (idx_t)16) {
        return base[n];
    }

    /* We keep a circular buffer of the last 16 computed values.  This allows
       us to compute the recurrence for any n without allocating or freeing
       memory. */
    seq_t ring[16];
    for (int i = 0; i < 16; ++i) {
        ring[i] = base[i];
    }

    /* Compute successive terms up to n using the linear recurrence. */
    for (size_t i = 16; i <= (size_t)n; ++i) {
        size_t idx = i & 15;                     /* i mod 16 */
        size_t idxm2 = (i - 2) & 15;             /* (i-2) mod 16 */
        size_t idxm6 = (i - 6) & 15;             /* (i-6) mod 16 */
        size_t idxm8 = (i - 8) & 15;             /* (i-8) mod 16 */

        /* a(i) = 5*a(i-2) - 20*a(i-6) + 16*a(i-8) */
        seq_t val = (seq_t)5 * ring[idxm2]
                    - (seq_t)20 * ring[idxm6]
                    + (seq_t)16 * ring[idxm8];

        ring[idx] = val;
    }

    return ring[n & 15];
}
