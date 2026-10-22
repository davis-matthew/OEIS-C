/*
 * A371763  Triangle read by rows: Trace of the Akiyama-Tanigawa algorithm for
 *          powers x^2.
 * https://oeis.org/A371763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattened triangle of T(n,k) * where T(n,k) = n^2 if n ==
 *            k * (k+1)*(2^(n-k)*(k+2)-3) otherwise * The sequence a(n)
 *            enumerates the triangle row‑by‑row, i.e. * a(0)=T(0,0),
 *            a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0),
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the row r that contains the n‑th element */
    seq_t tn = (seq_t)n;
    seq_t disc = 8 * tn + 1;                /* discriminant of r(r+1)/2 <= n */
    seq_t sq = floor_sqrt(disc);            /* floor(sqrt(8n+1))          */
    seq_t r = (sq - 1) / 2;                 /* floor((sqrt(8n+1)-1)/2)   */

    /* offset of the element within that row */
    seq_t tri_before = r * (r + 1) / 2;
    seq_t k = tn - tri_before;              /* 0 <= k <= r               */

    /* compute T(r,k) */
    if (k == r) {
        return r * r;                       /* diagonal entry            */
    } else {
        /* term = (k+1)*(2^(r-k)*(k+2)-3)   */
        idx_t exp = (idx_t)(r - k);          /* exponent fits in idx_t    */
        seq_t pow_term = pow2(exp);          /* 2^(r-k)                    */
        seq_t part = (k + 2) * pow_term;     /* (k+2)*2^(r-k)             */
        part = part - 3;
        return (k + 1) * part;
    }
}
