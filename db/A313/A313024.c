/*
 * A313024  Coordination sequence Gal.6.119.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordinator sequence for Gal.6.119.4
 *
 * Recurrence:  a(0)=1, a(1)=4.
 * For n>=2
 *     a(n) = a(n-1) + 4 + d(n)
 * where d(n)=1 if n%11 is 2,3,9 or 10; otherwise d(n)=0.
 *
 * Equivalently:
 *     a(n) = 4*n + cnt(n)   (n>=1)
 * where cnt(n) is the number of integers k in [2,n] with k%11 in {2,3,9,10}.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* Count how many k in [2,n] have k%11 == 2,3,9 or 10 */
    idx_t full_cycles = (n - 2) / 11;          /* each cycle gives 4 contributions   */
    idx_t rem         = (n - 2) % 11;          /* remainder 0..10                     */
    seq_t cnt = (seq_t)full_cycles * 4;        /* full cycles contribution             */

    /* add contributions from the incomplete cycle */
    if (rem == 0)            cnt += 1;   /* 2 */
    else if (rem == 1)       cnt += 2;   /* 2,3 */
    else if (rem <= 6)       cnt += 2;   /* currents 4–6 add nothing */
    else if (rem == 7)       cnt += 3;   /* 2,3,9 */
    else /* rem==8,9,10 */   cnt += 4;   /* 2,3,9,10 */

    /* Final value: a(n) = 4*n + cnt */
    return (seq_t)(4 * n + cnt);
}
