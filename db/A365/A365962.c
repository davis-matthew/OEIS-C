/*
 * A365962  Triangle read by rows: coefficients in expansion of Asveld's
 *          polynomials Pi_j(x).
 * https://oeis.org/A365962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient T(r,c) of Asveld's polynomials, where r = max
 *            m * such that (m+1)(m+2)/2 ≤ n, c = n – m(m+1)/2, and * T(r,c) =
 *            r! / c! · Fibonacci(r−c+3) with T(r,r)=1
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 65, u64 n <= 170, u128 n <= 464,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* find row r */
    idx_t r = 0;
    unsigned __int128 n128 = n;
    for (;; ++r) {
        unsigned __int128 s = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (s > n128) break;
    }
    /* column index c */
    idx_t c = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* T(r,c) */
    if (r == c) return 1;

    seq_t prod = 1;
    for (idx_t i = c + 1; i <= r; ++i) {
        prod = prod * (seq_t)i;
    }
    seq_t fib_val = fib(r - c + 3);
    return prod * fib_val;
}
