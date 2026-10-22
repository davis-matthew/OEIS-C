/*
 * A370778  Number of non-congruent integer triangles ABC with largest side BC
 *          of length n whose opposite vertex A lies inside or on the closed
 *          boundary formed by BC and a locus of points A' such that the
 *          triangle A'BC has tan A'/2 + tan B/2 + tan C/2 = 2.
 * https://oeis.org/A370778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 1 ≤ a ≤ b ≤ n-1 : a + b > n and ( (b+n-a)(a+n-b) +
 *            (b+n-a)(a+b-n) + (a+n-b)(a+b-n) )^2 ≥ 4 (
 *            (b+n-a)+(a+n-b)+(a+b-n) ) (b+n-a)(a+n-b)(a+b-n) }|
 * Range:     u8 n <= 54, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count integer triangles ABC with largest side BC of length n,
   such that the apex A lies inside or on the boundary defined by
   tan(A'/2)+tan(B/2)+tan(C/2)=2.  Counting is done over non-congruent
   triangles with sides (a,b,n) where 1 ≤ a ≤ b ≤ n-1 and a + b > n. */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    seq_t count = 0;
    for (idx_t a = 1; a <= n - 1; ++a) {
        for (idx_t b = a; b <= n - 1; ++b) {
            if (a + b <= n) continue;          /* triangle inequality */
            /* X = b + n - a,  Y = a + n - b,  Z = a + b - n, all positive */
            idx_t X = b + n - a;
            idx_t Y = a + n - b;
            idx_t Z = a + b - n;
            /* use unsigned __int128 for safe intermediate calculations */
            unsigned __int128 xy = (unsigned __int128)X * Y;
            unsigned __int128 xz = (unsigned __int128)X * Z;
            unsigned __int128 yz = (unsigned __int128)Y * Z;
            unsigned __int128 sumXY = xy + yz + xz;          /* (XY + XZ + YZ) */
            unsigned __int128 lhs = sumXY * sumXY;           /* (XY + XZ + YZ)^2 */
            unsigned __int128 xyz = xy * Z;                 /* X * Y * Z */
            unsigned __int128 rhs = (unsigned __int128)4 * (X + Y + Z) * xyz;
            if (lhs >= rhs)  /* sum of tan(half-angles) ≥ 2  (inside or on boundary) */
                count += 1;
        }
    }
    return count;
}
