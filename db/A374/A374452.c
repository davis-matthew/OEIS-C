/*
 * A374452  Iterated rascal triangle R3: T(n,k) = Sum_{m=0..3}
 *          binomial(n-k,m)*binomial(k,m).
 * https://oeis.org/A374452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A374452
 *
 * a(n) is the flat (row‑major) enumeration of the triangle
 * T(n,k) defined by
 *
 *     T(n,k) = Σ_{m=0..3} binomial(n-k,m) * binomial(k,m)
 *
 * Thus the first terms are
 *   a(0)=T(0,0)=1,
 *   a(1)=T(1,0)=1, a(2)=T(1,1)=1,
 *   a(3)=T(2,0)=1,
 *   a(4)=T(2,1)=2, a(5)=T(2,2)=1, ...
 *
 * The routine below converts the flat index n into the
 * corresponding row r and column k, and evaluates T(r,k)
 * directly, using the supplied generic binomial routine.
 */
#include <stdint.h>
#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)(8*n + 1)) - 1.0) / 2.0);

    /* Correct possible off‑by‑one caused by double rounding */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n)    --r;

    /* Column within the row */
    idx_t k = n - r * (r + 1) / 2;

    seq_t result = 0;
    idx_t r_minus_k = r - k;

    /* Compute T(r,k) = Σ_{m=0..3} binomial(r-k,m) * binomial(k,m) */
    for (idx_t m = 0; m <= 3; ++m) {
        if (m > r_minus_k) break;
        if (m > k)         break;
        seq_t term = binomial(r_minus_k, m);
        term *= binomial(k, m);
        result += term;
    }
    return result;
}
