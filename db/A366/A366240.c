/*
 * A366240  G.f. A(x) satisfies A(x) = 1 + x + x*(1 + x)^3*A(x)^3.
 * https://oeis.org/A366240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*k+1,n-k) *
 *            binomial(3*k,k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* the utilities are available through the build system */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial arguments are non‑negative */
        seq_t bin1 = binomial(5 * k + 1, n - k);
        seq_t bin2 = binomial(3 * k, k);
        /* intermediate multiplication – for seq_t that is 64‑bit this
           may overflow for very large n, but for n where the result
           fits 64 bits the intermediate value is still < 2^64.  */
        seq_t prod  = bin1 * bin2;
        seq_t term  = prod / (2 * k + 1);
        sum += term;
    }
    return sum;
}
