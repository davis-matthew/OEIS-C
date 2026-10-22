/*
 * A316851  Consider integer triangles as listed in rows of table A316841.
 *          Sequence gives perimeters of these triangles in the same order.
 * https://oeis.org/A316851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerate integer triangles.  For each triangle (a,b,c) with
 * 1 ≤ a ≤ b ≤ c, a + b > c, we list the triangles in the order
 * of increasing largest side c.  For a fixed c we order by
 * increasing second side b, and for equal b by increasing smallest
 * side a.  The function returns the perimeter a+b+c of the n‑th
 * triangle (with a(1)=3). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but safety */

    /* Find largest side c so that the number of triangles with
     * largest side ≤ c-1 is less than n, but with ≤ c is ≥ n. */
    idx_t cum = 0;   /* count before current c */
    idx_t c   = 0;   /* current largest side */

    for (;;) {
        ++c;
        /* m = ceil((c+1)/2) = (c+2)/2 (integer division) */
        idx_t m = (c + 2) / 2;
        /* number of triangles with this c */
        idx_t f = m * (c - m + 1);
        if (cum + f >= n) break;
        cum += f;
    }

    /* Index within the group for this c (0‑based) */
    idx_t idx_in_group = n - cum - 1;

    /* Determine the second side b and the smallest side a.
     * Triangles for this c are ordered by increasing b, and for a given
     * b by increasing a.  For each b the number of triangles equals
     * 2*b - c.  We find the b for which idx_in_group falls. */
    idx_t m = (c + 2) / 2;
    idx_t b;
    idx_t s = 0;   /* cumulative count of triangles for smaller b's */

    for (b = m; b <= c; ++b) {
        idx_t cnt = 2 * b - c;   /* number of triangles with this b */
        if (s + cnt > idx_in_group) break;
        s += cnt;
    }

    /* Smallest side a ranges from a_min = c - b + 1 to b */
    idx_t a_min = c - b + 1;
    idx_t a_val = a_min + (idx_in_group - s);

    /* Return the perimeter */
    return (seq_t)a_val + (seq_t)b + (seq_t)c;
}
