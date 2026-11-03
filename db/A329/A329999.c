/*
 * A329999  Beatty sequence for sqrt(x-1), where 1/sqrt(x-1) + 1/sqrt(x+1) =
 *          1.
 * https://oeis.org/A329999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * sqrt(x-1)), where x satisfies
 *            1/sqrt(x-1)+1/sqrt(x+1)=1. Using the relation alpha = sqrt(x-1)
 *            = 1 + δ with δ≈0.783010..., the sequence can be written as a(n)
 *            = n + floor(n * δ). The value δ is the unique real root of δ^4 +
 *            2δ^3 + 2δ^2 - 2δ - 1 = 0, lying in (0,1). The floor of nδ...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t floor_n_delta(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;        /* upper midpoint */

        /* Evaluate the polynomial
           G(m,n) = m^4 + 2 m^3 n + 2 m^2 n^2 - 2 m n^3 - n^4
           The sign of G(m,n) determines whether m/n < δ  (<0) or > δ (>0). */
        __int128 m = mid;
        __int128 N = n;

        __int128 term1 = m * m * m * m;                       /* m^4 */
        __int128 term2 = (__int128)2 * m * m * m * N;          /* 2 m^3 n */
        __int128 term3 = (__int128)2 * m * m * N * N;          /* 2 m^2 n^2 */
        __int128 term4 = (__int128)2 * m * N * N * N;          /* 2 m n^3 */
        __int128 term5 = N * N * N * N;                        /* n^4 */

        __int128 g = term1 + term2 + term3 - term4 - term5;

        if (g <= 0)   /* mid/n <= δ  → keep low = mid */
            low = mid;
        else          /* mid/n > δ  → decrease high */
            high = mid - 1;
    }
    return low;   /* floor(n * δ) */
}

seq_t a(idx_t n)
{
    /* a(n) = n + floor(n * δ)  with δ ≈ 0.783010…  */
    idx_t k = floor_n_delta(n);
    return (seq_t)n + (seq_t)k;
}
