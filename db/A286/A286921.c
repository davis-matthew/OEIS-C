/*
 * A286921  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 10 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A286921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of pattern classes in the [n,m]-rectangular grid
 *            with 10 colors, * where the triangle is read by rows and a(n)
 *            corresponds to T(n,m) with * n and m derived from the index n as
 *            described in OEIS A286921
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Handle the trivial first entry */
    if (n == 0)
        return (seq_t)1;

    /* Convert index n into row r and column m
     *   r = floor((sqrt(1 + 8n) - 1) / 2)
     *   m = n - r*(r+1)/2   (0 ≤ m ≤ r)
     */
    double d = sqrt((double)(1 + 8 * (double)n));
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    idx_t prev_sum = r * (r + 1) / 2;
    idx_t m = n - prev_sum;

    /* Common exponent values */
    idx_t mn   = r * m;          /* m*n  */
    idx_t half = mn / 2;         /* floor(m*n/2) */

    bool r_even = ((r & 1) == 0);
    bool m_even = ((m & 1) == 0);

    seq_t result;
    seq_t p_half = pow_uint(10, half);   /* 10^(mn/2) used several times */

    if (r_even && m_even) {
        /* T(n,m) = (10^(mn) + 3*10^(mn/2)) / 4 */
        seq_t p1 = pow_uint(10, mn);
        seq_t p2 = p_half * 3;
        result = (p1 + p2) / 4;
    } else if (r_even && !m_even) {
        /* T(n,m) = (10^(mn) + 10^((mn+r)/2) + 2*10^(mn/2)) / 4 */
        seq_t p1 = pow_uint(10, mn);
        seq_t p2 = pow_uint(10, (mn + r) / 2);
        seq_t p3 = p_half * 2;
        result = (p1 + p2 + p3) / 4;
    } else if (!r_even && m_even) {
        /* T(n,m) = (10^(mn) + 10^((mn+m)/2) + 2*10^(mn/2)) / 4 */
        seq_t p1 = pow_uint(10, mn);
        seq_t p2 = pow_uint(10, (mn + m) / 2);
        seq_t p3 = p_half * 2;
        result = (p1 + p2 + p3) / 4;
    } else { /* r odd, m odd */
        /* T(n,m) = (10^(mn) + 10^((mn+r)/2) + 10^((mn+m)/2)
                   + 10^((mn+1)/2)) / 4 */
        seq_t p1 = pow_uint(10, mn);
        seq_t p2 = pow_uint(10, (mn + r) / 2);
        seq_t p3 = pow_uint(10, (mn + m) / 2);
        seq_t p4 = pow_uint(10, (mn + 1) / 2);
        result = (p1 + p2 + p3 + p4) / 4;
    }

    return result;
}
