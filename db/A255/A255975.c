/*
 * A255975  Rectangular array T(i,j) read by downwards antidiagonals: an
 *          interspersion associated with the fractal sequence A022328.
 * https://oeis.org/A255975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 171, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255975: position of 2^i*3^j in the sorted list of all 2^k*3^l (k,l≥0),
   where (i,j) is the pair obtained by reading the triangular array
   T(i,j) downwards antidiagonally.  The function computes that rank
   directly using logarithms.  */
#include <stdint.h>
#include <math.h>

extern seq_t a(idx_t n)
{
    /* Find diagonal d that contains position n. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n) ++d;
    idx_t prev = d * (d + 1) / 2;          /* count of elements before diag d */
    idx_t pos  = n - prev - 1;             /* zero‑based position in diag d  */
    idx_t i  = pos;                        /* row index  */
    idx_t j  = d - pos;                    /* column index */

    /* Compute rank of 2^i * 3^j.  */
    const long double LOG2_3 = log2l(3.0L);   /* log2(3) */
    const long double EPS   = 1e-12L;         /* small safety margin */

    long double target_log = (long double)i + (long double)j * LOG2_3;
    idx_t max_k = (idx_t)floorl(target_log + EPS);
    seq_t rank = 0;

    for (idx_t k = 0; k <= max_k; ++k) {
        long double rem = target_log - (long double)k;
        if (rem < -EPS) break;
        long double lmax_f = rem / LOG2_3 + EPS;
        idx_t lmax = (idx_t)floorl(lmax_f);
        rank += (seq_t)(lmax + 1);   /* l ranges from 0 to lmax inclusive */
    }

    return rank;
}
