/*
 * A340899  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(4,n) (with n at least 4) missing three edges, where all
 *          three removed edges are incident to the same vertex in the 4-point
 *          set.
 * https://oeis.org/A340899
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(n) = 26*a(n-1) - 196*a(n-2) + 486*a(n-3) - 315*a(n-4)
   Base values: a(4)=2426, a(5)=57152, a(6)=1014458, a(7)=16353152
   (a(n) for n<4 is undefined, this implementation returns 0).
*/
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0; /* undefined for n < 4 */

    /* initialise base values */
    seq_t a0 = (seq_t)2426;          /* a(4) */
    seq_t a1 = (seq_t)57152;         /* a(5) */
    seq_t a2 = (seq_t)1014458;       /* a(6) */
    seq_t a3 = (seq_t)16353152;      /* a(7) */

    /* directly return if the requested index is within the pre‑computed
       range to avoid unnecessary work. */
    if (n == 4) return a0;
    if (n == 5) return a1;
    if (n == 6) return a2;
    if (n == 7) return a3;

    /* iterate using the linear recurrence until n is reached */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* compute next term */
        seq_t a_next = (seq_t)26 * a3
                       - (seq_t)196 * a2
                       + (seq_t)486 * a1
                       - (seq_t)315 * a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }

    return a3;   /* a(n) */
}
