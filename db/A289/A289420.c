/*
 * A289420  Number of Dyck paths of semilength n and height exactly 7.
 * https://oeis.org/A289420
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 20, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   compute_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_leq.h"
/* Implementation: a(n) = number of Dyck paths of semilength n and
   height exactly 7.
   For each n ≥ 7 we count Dyck paths whose maximum height is ≤ 7 and
   ≤ 6 with a standard dynamic‑programming walk on up/down steps.
   The desired count is f(n,7) – f(n,6).  Initial values:
   a(7)=1, a(8)=13, a(9)=102, a(10)=629, a(11)=3366, a(12)=16422, a(13)=75140. */

seq_t a(idx_t n)
{
    if (n < 7) return 0;          /* behavior below the offset is irrelevant */
    seq_t le7 = compute_leq(n, 7); /* paths with max height ≤ 7   */
    seq_t le6 = compute_leq(n, 6); /* paths with max height ≤ 6   */
    return le7 - le6;             /* exactly height 7            */
}
